/**
 *
 * Phantom compiler regression test.
 * 
 * Misc - math, control, etc.
 *
**/



package .ru.dz.test;


attribute const * ->!;

import .internal."thread";

class basetest
{
	var v : int;
	
	int basemethod(var a: int) {
		var avar : int;
		avar = a + 10;
		avar = avar + 10;
		return avar;
	}
};


class childtest extends basetest
{
	void method()
	{
		basemethod(v);
	}
};


class utilites {
    var _internal_counter : int;
    var _bootObject;
   
    void init( var bootObject ) {
        // NOTE : This should be removed to real random number retrieving
        _internal_counter = 0;
        _bootObject = bootObject;
    }

    int generateRandom( var minValue : int, var maxValue : int ) {
        if ( maxValue <= minValue ) {
            throw "ArgumentException";
        }
        //
        _internal_counter = _internal_counter + 11745 - ( _internal_counter / 5 );
        if ( _internal_counter > 60000 ) {
            _internal_counter = _internal_counter - 50000;
        }
        //
        var divided : int;
        divided = _internal_counter / ( maxValue - minValue + 1 );
        return ( minValue + _internal_counter - divided * ( maxValue - minValue + 1 ) );
    }
   
};

class regression_tests
{
	var boot_object;
	var boot_success : int;
	var i : int;
	var j : int;
	var test_array : .internal.string [];
	var int_array : .internal.int [];

	var _utilities : utilites;

	void regression_tests()
	{
	}

	int methodReturningInt() { return 0; }

	void failpoint()
	{
	
	_utilities.generateRandom(0, 50).toString();
	
var ch : string;
//string ch;

	ch = i.toString();


	}


	void run (var _boot_object @const )
	{
	boot_object = _boot_object;
	boot_success = 0;

	print("Phantom language regression tests are running\n");

	flow_test();
	math_test();
	array_test();
	var_alloc_test(2);
	//test_specials();
	}

	// ---------------------------------------------------------------------
	// Helpers
	// ---------------------------------------------------------------------

	void print( var input : string )
	{
	boot_object.16(input);
	}


	// ---------------------------------------------------------------------
	// test arrays
	// ---------------------------------------------------------------------

	void array_test()
	{
	print("Checking arrays... ");

//	test_array = new void[]();
	test_array = new string[]();

	test_array[0] = "zero";
	test_array[2] = "two";
	test_array[1] = "one";

	print("Beeping: ");
	i = 3;
	while( i )
		{
		i = i - 1;
		print("beep "); print(test_array[i]); print("! ");
		}
	print("\n");


	int_array[0] = 0;
	int_array[2] = 2;
	int_array[1] = 1;

	i = 3;
	while( i )
		{
		i = i - 1;
		if( int_array[i] != i )
			{
			print("array error: array["); print(i.toString());
			print("] == "); print(int_array[i].toString());
			throw "array error";
			}
		}


	print("passed\n");
	}

	// ---------------------------------------------------------------------
	// test basic math
	// ---------------------------------------------------------------------

	void math_test()
	{
	print("Checking int math... ");

	if( 2+2 != 4 ) throw "int + math error";
	if( 2-2 != 0 ) throw "int - math error";
	if( 2/2 != 1 ) throw "int / math error";
	if( 3*3 != 9 ) throw "int * math error";

	if( 3 < 2 ) throw "int < error";
	if( 5 > 6 ) throw "int > error";

	if( 3 <= 2 ) throw "int <= error";
	if( 5 >= 6 ) throw "int >= error";

	if( !(3 <= 3) ) throw "int <= error";
	if( !(5 >= 5) ) throw "int >= error";

	if( 0 || 0 )   throw "int || math error";
	if( 1 || 0 ) {} else  throw "int || math error";
	if( 0 || 1 ) {} else  throw "int || math error";
	if( 1 || 1 ) {} else  throw "int || math error";

	if( 0 && 0 )   throw "int && math error";
	if( 1 && 0 )   throw "int && math error";
	if( 0 && 1 )   throw "int && math error";
	if( 1 && 1 ) {} else  throw "int && math error";

	if( 0 | 0 )   throw "int | math error";
	if( 1 | 0 ) {} else  throw "int | math error";
	if( 0 | 1 ) {} else  throw "int | math error";
	if( 1 | 1 ) {} else  throw "int | math error";

	if( 0 & 0 )   throw "int & math error";
	if( 1 & 0 )   throw "int & math error";
	if( 0 & 1 )   throw "int & math error";
	if( 1 & 1 ) {} else  throw "int & math error";

	print("passed\n");
	}

	// ---------------------------------------------------------------------
	// test flow control
	// ---------------------------------------------------------------------

	void flow_test()
	{
	print("Checking flow control operators... ");

	if( 0 ) throw "if(0) error";
	if( 0 == 1 ) throw "if or == error";
	if( 0 != 0 ) throw "if or != error";

	// try to check that if never execs 'true' condition operator
	//print("try to check that if never execs 'true' condition operator... ");

	j = 0;
	if( 1 ) j = 1;
	if( j == 1 ) {} // empty
	else
	  throw "if(1) error";

	// test do/while
	i = 3;
	j = 0;
	do { j = j + 1; } while(i) i = i - 1;

	if( j != 4 ) throw "do/while error";

	i = 2;
	switch(i)
	{
	case 1:  throw "switch fall in error";
	case 2:  break;
	case 3:  throw "switch fall out error";
	default: throw "switch dafault error";
	}

	print("passed\n");
	}


	// ---------------------------------------------------------------------
	// Stack allocations
	// ---------------------------------------------------------------------

	var stack_test_temp : int;

	void var_alloc_test(var input : int)
	{
	var stack : int;

	if(input != 2) throw "parameter overwrite";
	stack = 3;
	if(input != 2) throw "parameter overwrite by stack var";
	input = 4;
	if(stack != 3) throw "stack overwrite by parameter var";

	stack_test_temp = 5;

	if(stack != 3) throw "auto var overwrite by stack op";
	if(input != 4) throw "parameter overwrite by stack op";

	}


};



class second_test extends regression_tests
{

};



