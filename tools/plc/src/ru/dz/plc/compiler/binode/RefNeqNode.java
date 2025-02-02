package ru.dz.plc.compiler.binode;

import java.io.IOException;

import ru.dz.phantom.code.Codegen;
import ru.dz.plc.compiler.CodeGeneratorState;
import ru.dz.plc.compiler.PhantomType;
import ru.dz.plc.compiler.node.Node;
import ru.dz.plc.util.PlcException;

/**
 * <p>Ref nonequality cmp node.</p>
 * <p>Copyright: Copyright (c) 2004-2009 Dmitry Zavalishin</p>
 * <p>Company: <a href="http://dz.ru/en">Digital Zone</a></p>
 * @author dz
 */


public class RefNeqNode extends BiNode 
{
	public RefNeqNode( Node l, Node r) {    super(l,r);  }
	public String toString()  {    return ":!=";  }
	public boolean is_on_int_stack() { return true; }
	public boolean args_on_int_stack() { return false; }

	@Override
	public PhantomType find_out_my_type() throws PlcException
	{
		return PhantomType.getInt();
	}	
	
	protected void generate_my_code(Codegen c, CodeGeneratorState s) throws IOException, PlcException {
		c.emit_ptr_neq();
	}
}