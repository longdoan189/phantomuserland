/**
 *
 * Phantom OS
 *
 * Copyright (C) 2005-2011 Dmitry Zavalishin, dz@dz.ru
 *
 * Deferred procedure calls. Quite old code, not safe in SMP,
 * needs to be replaced completely with something else.
 *
 *
 **/

#ifndef KERNEL_DPC_H
#define KERNEL_DPC_H

#include <assert.h>
#include <spinlock.h>
#include <hal.h>


//---------------------------------------------------------------------------
// Internals
//---------------------------------------------------------------------------

extern hal_cond_t   	dpc_thread_sleep_stone;
extern volatile int	dpc_stop_request;
extern volatile int     dpc_init_ok;

typedef struct dpc_request
{
    struct dpc_request *        prev;
    struct dpc_request *        next;
    void                    	(*func)( void * );
    void *                  	arg;
    int                    	go;

} dpc_request;

typedef struct dpc_request dpc_request_t;

extern hal_spinlock_t         dpc_request_lock;

extern struct dpc_request *    dpc_request_first;


//---------------------------------------------------------------------------
// Interface
//---------------------------------------------------------------------------

void dpc_init(void);
void dpc_finish(void);
void dpc_panic(void);

// called under spinlock
static __inline__
void dpc_request_move_to_beginning(dpc_request *me)
{
    if( (dpc_stop_request) || (!dpc_init_ok) )
        dpc_panic();

    if(dpc_request_first == me)
        return;

    if( me->prev ) { me->prev->next = me->next; }
    if( me->next ) { me->next->prev = me->prev; }

    if( dpc_request_first ) dpc_request_first->prev = me;
    me->next = dpc_request_first;
    me->prev = 0;
    dpc_request_first = me;
}


static __inline__
void
dpc_request_init(dpc_request *me, void (*_func)(void *))
{
    me->go = 0;
    me->func = _func;
    me->arg = 0;
    me->prev = me->next = 0;

    int ie = hal_save_cli();
    spinlock_lock( &dpc_request_lock, "dpc_request c'tor" );
    dpc_request_move_to_beginning(me);
    spinlock_unlock( &dpc_request_lock, "dpc_request c'tor" );
    if( ie ) hal_sti();
}

//void dpc_request_trigger(dpc_request *me ) { trigger(0); }
static __inline__
void dpc_request_trigger(dpc_request *me, void *_arg)
{
    if( (dpc_stop_request) || (!dpc_init_ok) )
        dpc_panic();

    //spinlock_lock( &dpc_request_lock, "trigger_dpc" );
    int ie = hal_save_cli();

    if((me->arg || _arg) && me->go)	panic("double dpc trigger with arg");

    // fixed, but removed for now - we do not depend on it
    //dpc_request_move_to_beginning(me);

    me->arg = _arg;
    me->go = 1;
    //spinlock_unlock( &dpc_request_lock, "trigger_dpc" );
    if( ie ) hal_sti();

    hal_cond_signal( &dpc_thread_sleep_stone );
}



#endif // KERNEL_DPC_H
