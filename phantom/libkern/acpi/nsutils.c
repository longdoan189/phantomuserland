/******************************************************************************
 *
 * Module Name: nsutils - Utilities for accessing ACPI namespace, accessing
 *                        parents and siblings and Scope manipulation
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999 - 2011, Intel Corp.
 * All rights reserved.
 *
 * 2. License
 *
 * 2.1. This is your license from Intel Corp. under its intellectual property
 * rights.  You may have additional license terms from the party that provided
 * you this software, covering your right to use that party's intellectual
 * property rights.
 *
 * 2.2. Intel grants, free of charge, to any person ("Licensee") obtaining a
 * copy of the source code appearing in this file ("Covered Code") an
 * irrevocable, perpetual, worldwide license under Intel's copyrights in the
 * base code distributed originally by Intel ("Original Intel Code") to copy,
 * make derivatives, distribute, use and display any portion of the Covered
 * Code in any form, with the right to sublicense such rights; and
 *
 * 2.3. Intel grants Licensee a non-exclusive and non-transferable patent
 * license (with the right to sublicense), under only those claims of Intel
 * patents that are infringed by the Original Intel Code, to make, use, sell,
 * offer to sell, and import the Covered Code and derivative works thereof
 * solely to the minimum extent necessary to exercise the above copyright
 * license, and in no event shall the patent license extend to any additions
 * to or modifications of the Original Intel Code.  No other license or right
 * is granted directly or by implication, estoppel or otherwise;
 *
 * The above copyright and patent license is granted only if the following
 * conditions are met:
 *
 * 3. Conditions
 *
 * 3.1. Redistribution of Source with Rights to Further Distribute Source.
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification with rights to further distribute source must include
 * the above Copyright Notice, the above License, this list of Conditions,
 * and the following Disclaimer and Export Compliance provision.  In addition,
 * Licensee must cause all Covered Code to which Licensee contributes to
 * contain a file documenting the changes Licensee made to create that Covered
 * Code and the date of any change.  Licensee must include in that file the
 * documentation of any changes made by any predecessor Licensee.  Licensee
 * must include a prominent statement that the modification is derived,
 * directly or indirectly, from Original Intel Code.
 *
 * 3.2. Redistribution of Source with no Rights to Further Distribute Source.
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification without rights to further distribute source must
 * include the following Disclaimer and Export Compliance provision in the
 * documentation and/or other materials provided with distribution.  In
 * addition, Licensee may not authorize further sublicense of source of any
 * portion of the Covered Code, and must include terms to the effect that the
 * license from Licensee to its licensee is limited to the intellectual
 * property embodied in the software Licensee provides to its licensee, and
 * not to intellectual property embodied in modifications its licensee may
 * make.
 *
 * 3.3. Redistribution of Executable. Redistribution in executable form of any
 * substantial portion of the Covered Code or modification must reproduce the
 * above Copyright Notice, and the following Disclaimer and Export Compliance
 * provision in the documentation and/or other materials provided with the
 * distribution.
 *
 * 3.4. Intel retains all right, title, and interest in and to the Original
 * Intel Code.
 *
 * 3.5. Neither the name Intel nor any other trademark owned or controlled by
 * Intel shall be used in advertising or otherwise to promote the sale, use or
 * other dealings in products derived from or relating to the Covered Code
 * without prior written authorization from Intel.
 *
 * 4. Disclaimer and Export Compliance
 *
 * 4.1. INTEL MAKES NO WARRANTY OF ANY KIND REGARDING ANY SOFTWARE PROVIDED
 * HERE.  ANY SOFTWARE ORIGINATING FROM INTEL OR DERIVED FROM INTEL SOFTWARE
 * IS PROVIDED "AS IS," AND INTEL WILL NOT PROVIDE ANY SUPPORT,  ASSISTANCE,
 * INSTALLATION, TRAINING OR OTHER SERVICES.  INTEL WILL NOT PROVIDE ANY
 * UPDATES, ENHANCEMENTS OR EXTENSIONS.  INTEL SPECIFICALLY DISCLAIMS ANY
 * IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGEMENT AND FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * 4.2. IN NO EVENT SHALL INTEL HAVE ANY LIABILITY TO LICENSEE, ITS LICENSEES
 * OR ANY OTHER THIRD PARTY, FOR ANY LOST PROFITS, LOST DATA, LOSS OF USE OR
 * COSTS OF PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, OR FOR ANY INDIRECT,
 * SPECIAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THIS AGREEMENT, UNDER ANY
 * CAUSE OF ACTION OR THEORY OF LIABILITY, AND IRRESPECTIVE OF WHETHER INTEL
 * HAS ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.  THESE LIMITATIONS
 * SHALL APPLY NOTWITHSTANDING THE FAILURE OF THE ESSENTIAL PURPOSE OF ANY
 * LIMITED REMEDY.
 *
 * 4.3. Licensee shall not export, either directly or indirectly, any of this
 * software or system incorporating such software without first obtaining any
 * required license or other approval from the U. S. Department of Commerce or
 * any other agency or department of the United States Government.  In the
 * event Licensee exports any such software from the United States or
 * re-exports any such software from a foreign destination, Licensee shall
 * ensure that the distribution and export/re-export of the software is in
 * compliance with all laws, regulations, orders, or other restrictions of the
 * U.S. Export Administration Regulations. Licensee agrees that neither it nor
 * any of its subsidiaries will export/re-export any technical data, process,
 * software, or service, directly or indirectly, to any country for which the
 * United States government or any agency thereof requires an export license,
 * other governmental approval, or letter of assurance, without first obtaining
 * such license, approval or letter.
 *
 *****************************************************************************/

#define __NSUTILS_C__

#include "acpi/acpi.h"
#include "acpi/accommon.h"
#include "acpi/acnamesp.h"
#include "acpi/amlcode.h"

#define _COMPONENT          ACPI_NAMESPACE
        ACPI_MODULE_NAME    ("nsutils")

/* Local prototypes */

static BOOLEAN
AcpiNsValidPathSeparator (
    char                    Sep);

#ifdef ACPI_OBSOLETE_FUNCTIONS
ACPI_NAME
AcpiNsFindParentName (
    ACPI_NAMESPACE_NODE     *NodeToSearch);
#endif


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsPrintNodePathname
 *
 * PARAMETERS:  Node            - Object
 *              Message         - Prefix message
 *
 * DESCRIPTION: Print an object's full namespace pathname
 *              Manages allocation/freeing of a pathname buffer
 *
 ******************************************************************************/

void
AcpiNsPrintNodePathname (
    ACPI_NAMESPACE_NODE     *Node,
    const char              *Message)
{
    ACPI_BUFFER             Buffer;
    ACPI_STATUS             Status;


    if (!Node)
    {
        AcpiOsPrintf ("[NULL NAME]");
        return;
    }

    /* Convert handle to full pathname and print it (with supplied message) */

    Buffer.Length = ACPI_ALLOCATE_LOCAL_BUFFER;

    Status = AcpiNsHandleToPathname (Node, &Buffer);
    if (ACPI_SUCCESS (Status))
    {
        if (Message)
        {
            AcpiOsPrintf ("%s ", Message);
        }

        AcpiOsPrintf ("[%s] (Node %p)", (char *) Buffer.Pointer, Node);
        ACPI_FREE (Buffer.Pointer);
    }
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsValidRootPrefix
 *
 * PARAMETERS:  Prefix          - Character to be checked
 *
 * RETURN:      TRUE if a valid prefix
 *
 * DESCRIPTION: Check if a character is a valid ACPI Root prefix
 *
 ******************************************************************************/

BOOLEAN
AcpiNsValidRootPrefix (
    char                    Prefix)
{

    return ((BOOLEAN) (Prefix == '\\'));
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsValidPathSeparator
 *
 * PARAMETERS:  Sep         - Character to be checked
 *
 * RETURN:      TRUE if a valid path separator
 *
 * DESCRIPTION: Check if a character is a valid ACPI path separator
 *
 ******************************************************************************/

static BOOLEAN
AcpiNsValidPathSeparator (
    char                    Sep)
{

    return ((BOOLEAN) (Sep == '.'));
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsGetType
 *
 * PARAMETERS:  Node        - Parent Node to be examined
 *
 * RETURN:      Type field from Node whose handle is passed
 *
 * DESCRIPTION: Return the type of a Namespace node
 *
 ******************************************************************************/

ACPI_OBJECT_TYPE
AcpiNsGetType (
    ACPI_NAMESPACE_NODE     *Node)
{
    ACPI_FUNCTION_TRACE (NsGetType);


    if (!Node)
    {
        ACPI_WARNING ((AE_INFO, "Null Node parameter"));
        return_UINT32 (ACPI_TYPE_ANY);
    }

    return_UINT32 ((ACPI_OBJECT_TYPE) Node->Type);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsLocal
 *
 * PARAMETERS:  Type        - A namespace object type
 *
 * RETURN:      LOCAL if names must be found locally in objects of the
 *              passed type, 0 if enclosing scopes should be searched
 *
 * DESCRIPTION: Returns scope rule for the given object type.
 *
 ******************************************************************************/

UINT32
AcpiNsLocal (
    ACPI_OBJECT_TYPE        Type)
{
    ACPI_FUNCTION_TRACE (NsLocal);


    if (!AcpiUtValidObjectType (Type))
    {
        /* Type code out of range  */

        ACPI_WARNING ((AE_INFO, "Invalid Object Type 0x%X", Type));
        return_UINT32 (ACPI_NS_NORMAL);
    }

    return_UINT32 ((UINT32) AcpiGbl_NsProperties[Type] & ACPI_NS_LOCAL);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsGetInternalNameLength
 *
 * PARAMETERS:  Info            - Info struct initialized with the
 *                                external name pointer.
 *
 * RETURN:      None
 *
 * DESCRIPTION: Calculate the length of the internal (AML) namestring
 *              corresponding to the external (ASL) namestring.
 *
 ******************************************************************************/

void
AcpiNsGetInternalNameLength (
    ACPI_NAMESTRING_INFO    *Info)
{
    const char              *NextExternalChar;
    UINT32                  i;


    ACPI_FUNCTION_ENTRY ();


    NextExternalChar = Info->ExternalName;
    Info->NumCarats = 0;
    Info->NumSegments = 0;
    Info->FullyQualified = FALSE;

    /*
     * For the internal name, the required length is 4 bytes per segment, plus
     * 1 each for RootPrefix, MultiNamePrefixOp, segment count, trailing null
     * (which is not really needed, but no there's harm in putting it there)
     *
     * strlen() + 1 covers the first NameSeg, which has no path separator
     */
    if (AcpiNsValidRootPrefix (*NextExternalChar))
    {
        Info->FullyQualified = TRUE;
        NextExternalChar++;

        /* Skip redundant RootPrefix, like \\_SB.PCI0.SBRG.EC0 */

        while (AcpiNsValidRootPrefix (*NextExternalChar))
        {
            NextExternalChar++;
        }
    }
    else
    {
        /* Handle Carat prefixes */

        while (*NextExternalChar == '^')
        {
            Info->NumCarats++;
            NextExternalChar++;
        }
    }

    /*
     * Determine the number of ACPI name "segments" by counting the number of
     * path separators within the string. Start with one segment since the
     * segment count is [(# separators) + 1], and zero separators is ok.
     */
    if (*NextExternalChar)
    {
        Info->NumSegments = 1;
        for (i = 0; NextExternalChar[i]; i++)
        {
            if (AcpiNsValidPathSeparator (NextExternalChar[i]))
            {
                Info->NumSegments++;
            }
        }
    }

    Info->Length = (ACPI_NAME_SIZE * Info->NumSegments) +
                    4 + Info->NumCarats;

    Info->NextExternalChar = NextExternalChar;
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsBuildInternalName
 *
 * PARAMETERS:  Info            - Info struct fully initialized
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Construct the internal (AML) namestring
 *              corresponding to the external (ASL) namestring.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiNsBuildInternalName (
    ACPI_NAMESTRING_INFO    *Info)
{
    UINT32                  NumSegments = Info->NumSegments;
    char                    *InternalName = Info->InternalName;
    const char              *ExternalName = Info->NextExternalChar;
    char                    *Result = NULL;
    UINT32                  i;


    ACPI_FUNCTION_TRACE (NsBuildInternalName);


    /* Setup the correct prefixes, counts, and pointers */

    if (Info->FullyQualified)
    {
        InternalName[0] = '\\';

        if (NumSegments <= 1)
        {
            Result = &InternalName[1];
        }
        else if (NumSegments == 2)
        {
            InternalName[1] = AML_DUAL_NAME_PREFIX;
            Result = &InternalName[2];
        }
        else
        {
            InternalName[1] = AML_MULTI_NAME_PREFIX_OP;
            InternalName[2] = (char) NumSegments;
            Result = &InternalName[3];
        }
    }
    else
    {
        /*
         * Not fully qualified.
         * Handle Carats first, then append the name segments
         */
        i = 0;
        if (Info->NumCarats)
        {
            for (i = 0; i < Info->NumCarats; i++)
            {
                InternalName[i] = '^';
            }
        }

        if (NumSegments <= 1)
        {
            Result = &InternalName[i];
        }
        else if (NumSegments == 2)
        {
            InternalName[i] = AML_DUAL_NAME_PREFIX;
            Result = &InternalName[(ACPI_SIZE) i+1];
        }
        else
        {
            InternalName[i] = AML_MULTI_NAME_PREFIX_OP;
            InternalName[(ACPI_SIZE) i+1] = (char) NumSegments;
            Result = &InternalName[(ACPI_SIZE) i+2];
        }
    }

    /* Build the name (minus path separators) */

    for (; NumSegments; NumSegments--)
    {
        for (i = 0; i < ACPI_NAME_SIZE; i++)
        {
            if (AcpiNsValidPathSeparator (*ExternalName) ||
               (*ExternalName == 0))
            {
                /* Pad the segment with underscore(s) if segment is short */

                Result[i] = '_';
            }
            else
            {
                /* Convert the character to uppercase and save it */

                Result[i] = (char) ACPI_TOUPPER ((int) *ExternalName);
                ExternalName++;
            }
        }

        /* Now we must have a path separator, or the pathname is bad */

        if (!AcpiNsValidPathSeparator (*ExternalName) &&
            (*ExternalName != 0))
        {
            return_ACPI_STATUS (AE_BAD_PARAMETER);
        }

        /* Move on the next segment */

        ExternalName++;
        Result += ACPI_NAME_SIZE;
    }

    /* Terminate the string */

    *Result = 0;

    if (Info->FullyQualified)
    {
        ACPI_DEBUG_PRINT ((ACPI_DB_EXEC, "Returning [%p] (abs) \"\\%s\"\n",
            InternalName, InternalName));
    }
    else
    {
        ACPI_DEBUG_PRINT ((ACPI_DB_EXEC, "Returning [%p] (rel) \"%s\"\n",
            InternalName, InternalName));
    }

    return_ACPI_STATUS (AE_OK);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsInternalizeName
 *
 * PARAMETERS:  *ExternalName           - External representation of name
 *              **Converted Name        - Where to return the resulting
 *                                        internal represention of the name
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Convert an external representation (e.g. "\_PR_.CPU0")
 *              to internal form (e.g. 5c 2f 02 5f 50 52 5f 43 50 55 30)
 *
 *******************************************************************************/

ACPI_STATUS
AcpiNsInternalizeName (
    const char              *ExternalName,
    char                    **ConvertedName)
{
    char                    *InternalName;
    ACPI_NAMESTRING_INFO    Info;
    ACPI_STATUS             Status;


    ACPI_FUNCTION_TRACE (NsInternalizeName);


    if ((!ExternalName)      ||
        (*ExternalName == 0) ||
        (!ConvertedName))
    {
        return_ACPI_STATUS (AE_BAD_PARAMETER);
    }

    /* Get the length of the new internal name */

    Info.ExternalName = ExternalName;
    AcpiNsGetInternalNameLength (&Info);

    /* We need a segment to store the internal  name */

    InternalName = ACPI_ALLOCATE_ZEROED (Info.Length);
    if (!InternalName)
    {
        return_ACPI_STATUS (AE_NO_MEMORY);
    }

    /* Build the name */

    Info.InternalName = InternalName;
    Status = AcpiNsBuildInternalName (&Info);
    if (ACPI_FAILURE (Status))
    {
        ACPI_FREE (InternalName);
        return_ACPI_STATUS (Status);
    }

    *ConvertedName = InternalName;
    return_ACPI_STATUS (AE_OK);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsExternalizeName
 *
 * PARAMETERS:  InternalNameLength  - Lenth of the internal name below
 *              InternalName        - Internal representation of name
 *              ConvertedNameLength - Where the length is returned
 *              ConvertedName       - Where the resulting external name
 *                                    is returned
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Convert internal name (e.g. 5c 2f 02 5f 50 52 5f 43 50 55 30)
 *              to its external (printable) form (e.g. "\_PR_.CPU0")
 *
 ******************************************************************************/

ACPI_STATUS
AcpiNsExternalizeName (
    UINT32                  InternalNameLength,
    const char              *InternalName,
    UINT32                  *ConvertedNameLength,
    char                    **ConvertedName)
{
    UINT32                  NamesIndex = 0;
    UINT32                  NumSegments = 0;
    UINT32                  RequiredLength;
    UINT32                  PrefixLength = 0;
    UINT32                  i = 0;
    UINT32                  j = 0;


    ACPI_FUNCTION_TRACE (NsExternalizeName);


    if (!InternalNameLength     ||
        !InternalName           ||
        !ConvertedName)
    {
        return_ACPI_STATUS (AE_BAD_PARAMETER);
    }

    /* Check for a prefix (one '\' | one or more '^') */

    switch (InternalName[0])
    {
    case '\\':
        PrefixLength = 1;
        break;

    case '^':
        for (i = 0; i < InternalNameLength; i++)
        {
            if (InternalName[i] == '^')
            {
                PrefixLength = i + 1;
            }
            else
            {
                break;
            }
        }

        if (i == InternalNameLength)
        {
            PrefixLength = i;
        }

        break;

    default:
        break;
    }

    /*
     * Check for object names. Note that there could be 0-255 of these
     * 4-byte elements.
     */
    if (PrefixLength < InternalNameLength)
    {
        switch (InternalName[PrefixLength])
        {
        case AML_MULTI_NAME_PREFIX_OP:

            /* <count> 4-byte names */

            NamesIndex = PrefixLength + 2;
            NumSegments = (UINT8)
                InternalName[(ACPI_SIZE) PrefixLength + 1];
            break;

        case AML_DUAL_NAME_PREFIX:

            /* Two 4-byte names */

            NamesIndex = PrefixLength + 1;
            NumSegments = 2;
            break;

        case 0:

            /* NullName */

            NamesIndex = 0;
            NumSegments = 0;
            break;

        default:

            /* one 4-byte name */

            NamesIndex = PrefixLength;
            NumSegments = 1;
            break;
        }
    }

    /*
     * Calculate the length of ConvertedName, which equals the length
     * of the prefix, length of all object names, length of any required
     * punctuation ('.') between object names, plus the NULL terminator.
     */
    RequiredLength = PrefixLength + (4 * NumSegments) +
                        ((NumSegments > 0) ? (NumSegments - 1) : 0) + 1;

    /*
     * Check to see if we're still in bounds.  If not, there's a problem
     * with InternalName (invalid format).
     */
    if (RequiredLength > InternalNameLength)
    {
        ACPI_ERROR ((AE_INFO, "Invalid internal name"));
        return_ACPI_STATUS (AE_BAD_PATHNAME);
    }

    /* Build the ConvertedName */

    *ConvertedName = ACPI_ALLOCATE_ZEROED (RequiredLength);
    if (!(*ConvertedName))
    {
        return_ACPI_STATUS (AE_NO_MEMORY);
    }

    j = 0;

    for (i = 0; i < PrefixLength; i++)
    {
        (*ConvertedName)[j++] = InternalName[i];
    }

    if (NumSegments > 0)
    {
        for (i = 0; i < NumSegments; i++)
        {
            if (i > 0)
            {
                (*ConvertedName)[j++] = '.';
            }

            (*ConvertedName)[j++] = InternalName[NamesIndex++];
            (*ConvertedName)[j++] = InternalName[NamesIndex++];
            (*ConvertedName)[j++] = InternalName[NamesIndex++];
            (*ConvertedName)[j++] = InternalName[NamesIndex++];
        }
    }

    if (ConvertedNameLength)
    {
        *ConvertedNameLength = (UINT32) RequiredLength;
    }

    return_ACPI_STATUS (AE_OK);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsValidateHandle
 *
 * PARAMETERS:  Handle          - Handle to be validated and typecast to a
 *                                namespace node.
 *
 * RETURN:      A pointer to a namespace node
 *
 * DESCRIPTION: Convert a namespace handle to a namespace node. Handles special
 *              cases for the root node.
 *
 * NOTE: Real integer handles would allow for more verification
 *       and keep all pointers within this subsystem - however this introduces
 *       more overhead and has not been necessary to this point. Drivers
 *       holding handles are typically notified before a node becomes invalid
 *       due to a table unload.
 *
 ******************************************************************************/

ACPI_NAMESPACE_NODE *
AcpiNsValidateHandle (
    ACPI_HANDLE             Handle)
{

    ACPI_FUNCTION_ENTRY ();


    /* Parameter validation */

    if ((!Handle) || (Handle == ACPI_ROOT_OBJECT))
    {
        return (AcpiGbl_RootNode);
    }

    /* We can at least attempt to verify the handle */

    if (ACPI_GET_DESCRIPTOR_TYPE (Handle) != ACPI_DESC_TYPE_NAMED)
    {
        return (NULL);
    }

    return (ACPI_CAST_PTR (ACPI_NAMESPACE_NODE, Handle));
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsTerminate
 *
 * PARAMETERS:  none
 *
 * RETURN:      none
 *
 * DESCRIPTION: free memory allocated for namespace and ACPI table storage.
 *
 ******************************************************************************/

void
AcpiNsTerminate (
    void)
{
    ACPI_OPERAND_OBJECT     *ObjDesc;


    ACPI_FUNCTION_TRACE (NsTerminate);


    /*
     * 1) Free the entire namespace -- all nodes and objects
     *
     * Delete all object descriptors attached to namepsace nodes
     */
    AcpiNsDeleteNamespaceSubtree (AcpiGbl_RootNode);

    /* Detach any objects attached to the root */

    ObjDesc = AcpiNsGetAttachedObject (AcpiGbl_RootNode);
    if (ObjDesc)
    {
        AcpiNsDetachObject (AcpiGbl_RootNode);
    }

    ACPI_DEBUG_PRINT ((ACPI_DB_INFO, "Namespace freed\n"));
    return_VOID;
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsOpensScope
 *
 * PARAMETERS:  Type        - A valid namespace type
 *
 * RETURN:      NEWSCOPE if the passed type "opens a name scope" according
 *              to the ACPI specification, else 0
 *
 ******************************************************************************/

UINT32
AcpiNsOpensScope (
    ACPI_OBJECT_TYPE        Type)
{
    ACPI_FUNCTION_TRACE_STR (NsOpensScope, AcpiUtGetTypeName (Type));


    if (!AcpiUtValidObjectType (Type))
    {
        /* type code out of range  */

        ACPI_WARNING ((AE_INFO, "Invalid Object Type 0x%X", Type));
        return_UINT32 (ACPI_NS_NORMAL);
    }

    return_UINT32 (((UINT32) AcpiGbl_NsProperties[Type]) & ACPI_NS_NEWSCOPE);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsGetNode
 *
 * PARAMETERS:  *Pathname   - Name to be found, in external (ASL) format. The
 *                            \ (backslash) and ^ (carat) prefixes, and the
 *                            . (period) to separate segments are supported.
 *              PrefixNode   - Root of subtree to be searched, or NS_ALL for the
 *                            root of the name space.  If Name is fully
 *                            qualified (first INT8 is '\'), the passed value
 *                            of Scope will not be accessed.
 *              Flags       - Used to indicate whether to perform upsearch or
 *                            not.
 *              ReturnNode  - Where the Node is returned
 *
 * DESCRIPTION: Look up a name relative to a given scope and return the
 *              corresponding Node.  NOTE: Scope can be null.
 *
 * MUTEX:       Locks namespace
 *
 ******************************************************************************/

ACPI_STATUS
AcpiNsGetNode (
    ACPI_NAMESPACE_NODE     *PrefixNode,
    const char              *Pathname,
    UINT32                  Flags,
    ACPI_NAMESPACE_NODE     **ReturnNode)
{
    ACPI_GENERIC_STATE      ScopeInfo;
    ACPI_STATUS             Status;
    char                    *InternalPath;


    ACPI_FUNCTION_TRACE_PTR (NsGetNode, ACPI_CAST_PTR (char, Pathname));


    if (!Pathname)
    {
        *ReturnNode = PrefixNode;
        if (!PrefixNode)
        {
            *ReturnNode = AcpiGbl_RootNode;
        }
        return_ACPI_STATUS (AE_OK);
    }

    /* Convert path to internal representation */

    Status = AcpiNsInternalizeName (Pathname, &InternalPath);
    if (ACPI_FAILURE (Status))
    {
        return_ACPI_STATUS (Status);
    }

    /* Must lock namespace during lookup */

    Status = AcpiUtAcquireMutex (ACPI_MTX_NAMESPACE);
    if (ACPI_FAILURE (Status))
    {
        goto Cleanup;
    }

    /* Setup lookup scope (search starting point) */

    ScopeInfo.Scope.Node = PrefixNode;

    /* Lookup the name in the namespace */

    Status = AcpiNsLookup (&ScopeInfo, InternalPath, ACPI_TYPE_ANY,
                ACPI_IMODE_EXECUTE, (Flags | ACPI_NS_DONT_OPEN_SCOPE),
                NULL, ReturnNode);
    if (ACPI_FAILURE (Status))
    {
        ACPI_DEBUG_PRINT ((ACPI_DB_EXEC, "%s, %s\n",
                Pathname, AcpiFormatException (Status)));
    }

    (void) AcpiUtReleaseMutex (ACPI_MTX_NAMESPACE);

Cleanup:
    ACPI_FREE (InternalPath);
    return_ACPI_STATUS (Status);
}
