/* Nic Pucci
 * TEST-DRIVER
*/

#include <stdio.h>
#include <stdlib.h>
#include "List.h"

/* MAKE SURE THESE ARE SMALLER THAN OR EQUAL TO MAX VALUES IN List.c */
#define MAX_TEST_NUM_NODES 500
#define MAX_TEST_NUM_LISTS 500

/* TEST RETURN CODES */
const int TEST_FAILED = -1;
const int TEST_UNTESTABLE = 0;
const int TEST_SUCCESS = 2;

/* TESTING DATA */
LIST *testListsPtrArr [ MAX_TEST_NUM_LISTS ];
int *testIntArrPtr [ MAX_TEST_NUM_NODES ];

/* TESTING UTILITY MESSAGES */
const char EMPTY_LISTS_MESSAGE [] = "There are no Lists";
const char EMPTY_NODES_MESSAGE [] = "There are no Nodes";
const char EMPTY_LISTS_AND_NODES_MESSAGE [] = "Empty Lists and Nodes";
const char NULL_VALUE_RETURNED_MESSAGE [] = "NULL value returned";
const char NOT_NULL_RETURNED_MESSAGE [] = "not NULL value returned";
const char ADD_FAILED_MESSAGE [] = "ADD FAILED";
const char WRONG_RETURN_VALUE_MESSAGE [] = "Wrong value return";
const char INCORRECT_COUNT_MESSAGE [] = "Incorrect count reported";
const char SUCCESSFUL_TEST_MESSAGE [] = "Complete";

/* DEBUG UTILITY FUNCTIONS */
void DEBUG_PRINT_LIST ( LIST *list ) {
	return;
	printf ( "\n-------------- DEBUG_PRINT_LIST\n" );
	if ( !list ) 
	{
		printf ( "list is NULL\n" );
		return;
	}

	printf ( "Info:\n" );
	printf ( "Current Capacity = %d\n" , list -> currentCapacity );

	if ( !list -> currentNodePtr ) 
	{
		printf ( "CURRENT = NULL\n" );
	}
	else 
	{
		int* currentValue = ( int* ) list -> currentNodePtr -> valuePtr;
		if ( currentValue ) 
		{	
			printf ( "CURRENT -> (item: %d) [%p]\n" , *currentValue , list -> currentNodePtr );
		}
		else 
		{
			printf ( "CURRENT -> (NULL) [%p]\n" , list -> currentNodePtr );
		}
	}

	enum CURRENT_NODE_STATE currentNodeState = list -> currentNodeState;
	if ( currentNodeState == BEFORE_HEAD ) 
	{
		printf ( "current node state = BEFORE_HEAD\n" );
	}

	else if ( currentNodeState == WITHIN_LIST ) 
	{
		printf ( "current node state = WITHIN_LIST\n" );
	}
	else if ( currentNodeState == AFTER_TAIL ) 
	{
		printf ( "current node state = AFTER_TAIL\n" );
	}

	if ( !list -> headNodePtr ) 
	{
		printf ( "HEAD = NULL\n" );
	}
	else 
	{
		int* headValue = ( int* ) list -> headNodePtr -> valuePtr;
		if ( headValue ) 
		{	
			printf ( "HEAD -> (item: %d) [%p]\n" , *headValue , list -> headNodePtr );
		}
		else 
		{
			printf ( "HEAD -> (NULL) [%p]\n" , list -> headNodePtr );
		}
	}

	if ( !list -> tailNodePtr ) 
	{
		printf ( "TAIL = NULL\n" );
	}
	else 
	{
		int* tailValue = ( int* ) list -> tailNodePtr -> valuePtr;
		if ( tailValue ) 
		{	
			printf ( "TAIL -> (item: %d) [%p]\n" , *tailValue , list -> tailNodePtr );
		}
		else 
		{
			printf ( "TAIL -> (NULL) [%p]\n" , list -> tailNodePtr );
		}
	}

	int currentCapacity = list -> currentCapacity;
	if ( currentCapacity <= 0 ) 
	{
		printf ( "HEAD -> NULL <- TAIL\n" );
		printf ( "----------------------------\n\n" );
		return;
	}

	printf ( "HEAD -> " );
	NODE *nextNode = list -> headNodePtr;
	int i = 0;
	while ( nextNode ) 
	{
		void *nextItem = nextNode -> valuePtr;
		if ( i != 0 ) 
		{
			printf ( "<- " );
		}
		if ( nextItem ) 
		{
			int value = *( ( int* ) nextItem );
			printf ( "(item %d: %d)" , i , value );
		}
		else 
		{
			printf ( "<- (item %d: NULL) ->" , i );
		}

		nextNode = nextNode -> nextNodePtr;
		if ( nextNode ) 
		{
			printf ( "-> " );
		}

		i++;
	}

	printf ( " <- TAIL\n" );

	printf ( "----------------------------\n\n" );
}

void DEBUG_PRINT_VOID_PTR ( void *voidPtr ) {
	if ( !voidPtr ) 
	{
		printf ( "NULL" );
		return;
	}

	int value = *( ( int* ) voidPtr );
	printf ( "%d", value );
}

/* TESTING UTILITY FUNCTIONS */
int CheckIfEmptyLists () 
{
	return MAX_TEST_NUM_LISTS <= 0;
}

int CheckIfEmptyNodes () 
{
	return MAX_TEST_NUM_NODES <= 0;
}

int CheckIfEmptyListsAndNodes () 
{
	return CheckIfEmptyNodes () && CheckIfEmptyLists () ;
}

int AssertEqualInts ( void *item1 , void *item2 ) 
{
	if ( !item1 || !item2 ) 
	{
		return 0;
	}

	int itemValue1 = *( int *) item1;
	int itemValue2 = *( int *) item2;

	return itemValue1== itemValue2;
}

void TEST_SUITE_PRINT_START ( const char testSuiteName [] ) 
{
	printf ( "\n\nStarting Test Suite: %s\n\n" , testSuiteName );
}

void TEST_SUITE_PRINT_RESULTS ( const char testSuiteName [] , int numTestsPassed , int numTestsFailed , int numTestsUntestable ) {
	 printf ( "-----RESULTS: %s-----\n" , testSuiteName );
	 printf ( "SUCCESS: %d, FAILED: %d, SKIPPED: %d\n\n" , numTestsPassed , numTestsFailed , numTestsUntestable );
}

void TEST_PRINT_RESULTS ( const char testName [] , const char testMessage [] , int testCode ) {
	 printf ( "Test: %s\n" , testName );
	 printf ( "Message: %s\n" , testMessage );

	 if ( testCode == TEST_SUCCESS ) 
	 {
	 	printf ( "Result: TEST_SUCCESS\n\n" );
	 }
	 else if ( testCode == TEST_UNTESTABLE ) 
	 {
	 	printf ( "Result: TEST_UNTESTABLE ... SKIPPING TEST\n\n" );
	 }
	 else if ( testCode == TEST_FAILED ) 
	 {
	 	printf ( "Result: TEST_FAILED\n\n" );
	 }
	 else 
	 {
	 	printf ( "\n\n" );
	 }
}

/* TESTING SETUP AND CLEAN UP FUNCTIONS */
void TEST_SETUP () 
{
	for ( int i = 0 ; i < MAX_TEST_NUM_NODES ; i++ ) 
	{
		int *value = (int *) malloc ( sizeof ( int ) );
		*value = i;

		testIntArrPtr [ i ] = value;
	}
}

void FreeItem ( void *item ) 
{
	if ( !item ) 
	{
		return;
	}

	for ( int j = 0 ; j < MAX_TEST_NUM_NODES ; j++ ) 
	{
		if ( testIntArrPtr [ j ] && *testIntArrPtr [ j ] == *( (int*) item) ) 
		{
			testIntArrPtr [ j ] = NULL;
		}
	}

	free ( item );
}

void TEST_TEAR_DOWN () 
{ 
	void ( *freeItemPtr ) () = &FreeItem;
	for ( int i = 0 ; i < MAX_TEST_NUM_LISTS ; i++ ) 
	{
		LIST *removeList = testListsPtrArr [ i ];
		ListFree ( removeList , freeItemPtr );
	}

	for ( int j = 0 ; j < MAX_TEST_NUM_NODES ; j++ ) 
	{
		if ( testIntArrPtr [ j ] ) 
		{
			free ( testIntArrPtr [ j ] );
			testIntArrPtr [ j ] = NULL;
		}
	}
}

/* TESTS */
int TEST_LIST_ADD () 
{
	char testName [] = "TEST_LIST_ADD";

	if ( CheckIfEmptyListsAndNodes () ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			EMPTY_LISTS_AND_NODES_MESSAGE , 
			TEST_UNTESTABLE 
		);
		return TEST_UNTESTABLE;
	}

	TEST_SETUP ();

	testListsPtrArr [ 0 ] = ListCreate ();
	LIST* list = testListsPtrArr [ 0 ];
	for ( int i = 0 ; i < MAX_TEST_NUM_NODES ; i++ ) 
	{
		ListAdd ( list , testIntArrPtr [ i ] );
	}

	ListFirst ( list );
	for ( int i = 0 ; i < MAX_TEST_NUM_NODES ; i++ ) 
	{
		void *value = ListCurr ( list );
		if ( !value ) 
		{
			TEST_PRINT_RESULTS ( 
				testName , 
				NULL_VALUE_RETURNED_MESSAGE , 
				TEST_FAILED 
			);
			TEST_TEAR_DOWN ();
			return TEST_FAILED;
		}

		int intValue = *( ( int *) value );
		if ( intValue != *testIntArrPtr [ i ] ) 
		{
			TEST_PRINT_RESULTS ( 
				testName , 
				WRONG_RETURN_VALUE_MESSAGE , 
				TEST_FAILED 
			);
			TEST_TEAR_DOWN ();
			return TEST_FAILED;
		}

		ListNext ( list );	
	}

	TEST_PRINT_RESULTS ( 
		testName , 
		SUCCESSFUL_TEST_MESSAGE , 
		TEST_FAILED 
	);

	TEST_TEAR_DOWN ();
	return TEST_SUCCESS;
}

int TEST_LIST_FIRST () 
{
	char testName [] = "TEST_LIST_FIRST";
	
	if ( CheckIfEmptyListsAndNodes () ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			EMPTY_LISTS_AND_NODES_MESSAGE , 
			TEST_UNTESTABLE 
		);
		return TEST_UNTESTABLE;
	}

	TEST_SETUP ();

	testListsPtrArr [ 0 ] = ListCreate ();
	LIST* list = testListsPtrArr [ 0 ];
	int *nullValuePtr = ( int *) ListFirst ( list );
	if ( nullValuePtr ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			NOT_NULL_RETURNED_MESSAGE , 
			TEST_FAILED 
		);

		TEST_TEAR_DOWN ();
		return TEST_FAILED;
	}

	int addSuccess = ListAdd ( list , testIntArrPtr [ 0 ] );
	if ( addSuccess == FAILURE_OP_CODE ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			ADD_FAILED_MESSAGE , 
			TEST_FAILED 
		);

		TEST_TEAR_DOWN ();
		return TEST_FAILED;
	}

	int *returnValue = ( int *) ListFirst ( list );
	if ( !returnValue ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			NULL_VALUE_RETURNED_MESSAGE , 
			TEST_FAILED 
		);

		TEST_TEAR_DOWN ();
		return TEST_FAILED;
	}

	if ( *returnValue != *testIntArrPtr [ 0 ] ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			WRONG_RETURN_VALUE_MESSAGE , 
			TEST_FAILED 
		);

		TEST_TEAR_DOWN ();
		return TEST_FAILED;
	}

	TEST_PRINT_RESULTS ( 
		testName , 
		SUCCESSFUL_TEST_MESSAGE , 
		TEST_SUCCESS 
	);

	TEST_TEAR_DOWN ();
	return TEST_SUCCESS;
}

int TEST_LIST_LAST () 
{
	char testName [] = "TEST_LIST_LAST";

	if ( CheckIfEmptyListsAndNodes () ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			EMPTY_LISTS_AND_NODES_MESSAGE , 
			TEST_UNTESTABLE 
		);
		return TEST_UNTESTABLE;
	}

	TEST_SETUP ();

	testListsPtrArr [ 0 ] = ListCreate ();
	LIST* list = testListsPtrArr [ 0 ];
	int *nullValuePtr = ( int *) ListFirst ( list );
	if ( nullValuePtr ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			NOT_NULL_RETURNED_MESSAGE , 
			TEST_FAILED 
		);
		return TEST_FAILED;
	}

	int addSuccess = ListAdd ( list , testIntArrPtr [ 0 ] );
	if ( addSuccess == FAILURE_OP_CODE ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			ADD_FAILED_MESSAGE , 
			TEST_FAILED 
		);
		return TEST_FAILED;
	}

	int *returnValue = ( int *) ListLast ( list );
	if ( !returnValue ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			NULL_VALUE_RETURNED_MESSAGE , 
			TEST_FAILED 
		);
		return TEST_FAILED;
	}

	if ( *returnValue != *testIntArrPtr [ 0 ] ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			WRONG_RETURN_VALUE_MESSAGE , 
			TEST_FAILED 
		);
		return TEST_FAILED;
	}

	TEST_PRINT_RESULTS ( 
		testName , 
		SUCCESSFUL_TEST_MESSAGE , 
		TEST_SUCCESS 
	);

	TEST_TEAR_DOWN ();
	return TEST_SUCCESS;
}

int TEST_MAX_LIST_TRIM_ALL_NODES () 
{
	char testName [] = "TEST_MAX_LIST_TRIM_ALL_NODES";
	if ( CheckIfEmptyListsAndNodes () ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			EMPTY_LISTS_AND_NODES_MESSAGE , 
			TEST_UNTESTABLE 
		);
		return TEST_UNTESTABLE;
	}

	TEST_SETUP ();

	testListsPtrArr [ 0 ] = ListCreate ();
	LIST *list = testListsPtrArr [ 0 ];

	for ( int i = 0 ; i < MAX_TEST_NUM_NODES ; i++ ) 
	{
		int addSuccess = ListAdd ( list , testIntArrPtr [ i ] );	
		if ( addSuccess == FAILURE_OP_CODE ) 
		{
			TEST_PRINT_RESULTS ( 
				testName , 
				ADD_FAILED_MESSAGE , 
				TEST_FAILED 
			);
			return TEST_FAILED;
		}
	}

	for ( int i = MAX_TEST_NUM_NODES - 1 ; i > 0 ; i-- ) 
	{
		int *returnValue = ( int *) ListTrim ( list );
		if ( !returnValue ) 
		{
			TEST_PRINT_RESULTS ( 
				testName , 
				NULL_VALUE_RETURNED_MESSAGE , 
				TEST_FAILED 
			);
			return TEST_FAILED;
		}

		if ( *returnValue != *testIntArrPtr [ i ] ) 
		{
			TEST_PRINT_RESULTS ( 
				testName , 
				WRONG_RETURN_VALUE_MESSAGE , 
				TEST_FAILED 
			);
			return TEST_FAILED;
		}
	}

	TEST_PRINT_RESULTS ( 
		testName , 
		SUCCESSFUL_TEST_MESSAGE , 
		TEST_SUCCESS 
	);

	TEST_TEAR_DOWN ();
	return TEST_SUCCESS;
}

int TEST_LIST_FIRST_AND_LAST () 
{
	char testName [] = "TEST_LIST_FIRST_AND_LAST";

	if ( CheckIfEmptyListsAndNodes () ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			EMPTY_LISTS_AND_NODES_MESSAGE , 
			TEST_UNTESTABLE 
		);
		return TEST_UNTESTABLE;
	}

	TEST_SETUP ();

	testListsPtrArr [ 0 ] = ListCreate ();
	LIST* list = testListsPtrArr [ 0 ];
	for ( int i = 0 ; i < MAX_TEST_NUM_NODES ; i++ ) 
	{
		int addSuccess = ListAdd ( list , testIntArrPtr [ i ] );
		if ( addSuccess == -1 ) {
			TEST_PRINT_RESULTS ( 
				testName , 
				ADD_FAILED_MESSAGE , 
				TEST_FAILED 
			);

			TEST_TEAR_DOWN ();
			return TEST_FAILED;
		}

	}

	ListFirst ( list );
	for ( int i = 0 ; i < MAX_TEST_NUM_NODES ; i++ ) 
	{
		void *value = ListCurr ( list );
		if ( !value ) 
		{
			TEST_PRINT_RESULTS ( 
				testName , 
				NULL_VALUE_RETURNED_MESSAGE , 
				TEST_FAILED 
			);

			TEST_TEAR_DOWN ();
			return TEST_FAILED;
		}

		int intValue = *( ( int *) value );
		if ( intValue != *testIntArrPtr [ i ] ) 
		{
			TEST_PRINT_RESULTS ( 
				testName , 
				WRONG_RETURN_VALUE_MESSAGE , 
				TEST_FAILED 
			);

			TEST_TEAR_DOWN ();
			return TEST_FAILED;
		}

		ListNext ( list );	
	}

	int *firstValue = ( int *) ListFirst ( list );
	if ( !firstValue ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			NULL_VALUE_RETURNED_MESSAGE , 
			TEST_FAILED 
		);
		return TEST_FAILED;
	}

	if ( *firstValue != *testIntArrPtr [ 0 ] ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			WRONG_RETURN_VALUE_MESSAGE , 
			TEST_FAILED 
		);
		return TEST_FAILED;
	}

	int *lastValue = ( int *) ListLast ( list );
	if ( !lastValue ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			NULL_VALUE_RETURNED_MESSAGE , 
			TEST_FAILED 
		);
		return TEST_FAILED;
	}

	if ( *lastValue != *testIntArrPtr [ MAX_TEST_NUM_NODES - 1 ] ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			WRONG_RETURN_VALUE_MESSAGE , 
			TEST_FAILED 
		);
		return TEST_FAILED;
	}

	TEST_PRINT_RESULTS ( 
		testName , 
		SUCCESSFUL_TEST_MESSAGE , 
		TEST_SUCCESS 
	);

	TEST_TEAR_DOWN ();
	return TEST_SUCCESS;
}

int TEST_LIST_COUNT () 
{
	char testName [] = "TEST_LIST_COUNT";

	if ( CheckIfEmptyLists () ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			EMPTY_LISTS_MESSAGE , 
			TEST_UNTESTABLE 
		);
		return TEST_UNTESTABLE;
	}

	TEST_SETUP ();

	testListsPtrArr [ 0 ] = ListCreate ();
	LIST* list = testListsPtrArr [ 0 ];

	// test all value counting up
	for ( int i = 0 ; i < MAX_TEST_NUM_NODES ; i++ ) 
	{
		int addSuccess = ListAdd ( list , testIntArrPtr [ i ] );
		if ( addSuccess == FAILURE_OP_CODE ) 
		{
			TEST_PRINT_RESULTS ( 
				testName , 
				ADD_FAILED_MESSAGE , 
				TEST_FAILED 
			);
			return TEST_FAILED;
		}
			
		int correctCountAfterAddition = i + 1;

		int count = ListCount ( list );
		if ( count != correctCountAfterAddition ) 
		{
			TEST_PRINT_RESULTS ( 
				testName , 
				INCORRECT_COUNT_MESSAGE , 
				TEST_FAILED 
			);
			return TEST_FAILED;
		}
	}

	// test all values counting down
	for ( int j = MAX_TEST_NUM_NODES - 1 ; j > 0 ; j-- ) 
	{
		ListTrim ( list );

		int correctCountAfterTrim = j;
		
		int count = ListCount ( list );
		if ( count != correctCountAfterTrim ) 
		{
			TEST_PRINT_RESULTS ( 
				testName , 
				INCORRECT_COUNT_MESSAGE , 
				TEST_FAILED 
			);
			return TEST_FAILED;
		}
	}

	TEST_PRINT_RESULTS ( 
		testName , 
		SUCCESSFUL_TEST_MESSAGE , 
		TEST_SUCCESS 
	);

	TEST_TEAR_DOWN ();
	return TEST_SUCCESS;
}

int TEST_ALL_LISTS_USED ()
{
	char testName [] = "TEST_ALL_LISTS_USED";

	TEST_SETUP ();

	for ( int i = 0 ; i < MAX_TEST_NUM_LISTS ; i++ ) 
	{
		int v = i;
		testListsPtrArr [ i ] = ListCreate ();
	}

	LIST *extraListBeyondMem = ListCreate ();
	if ( extraListBeyondMem ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			NOT_NULL_RETURNED_MESSAGE , 
			TEST_UNTESTABLE 
		);
	}

	TEST_PRINT_RESULTS ( 
		testName , 
		SUCCESSFUL_TEST_MESSAGE , 
		TEST_SUCCESS 
	);

	TEST_TEAR_DOWN ();
	return TEST_SUCCESS;
}

int TEST_ALL_NODES_USED_ONE_LIST ()
{
	char testName [] = "TEST_ALL_NODES_USED_ONE_LIST";

	if ( CheckIfEmptyListsAndNodes () ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			EMPTY_LISTS_AND_NODES_MESSAGE , 
			TEST_UNTESTABLE 
		);
		return TEST_UNTESTABLE;
	}

	TEST_SETUP (); 

	testListsPtrArr [ 0 ] = ListCreate ();
	LIST *list = testListsPtrArr [ 0 ];
	for ( int i = 0 ; i < MAX_TEST_NUM_NODES ; i++ ) 
	{
		int addSuccess = ListAdd ( list , testIntArrPtr [ i ] );
		if ( addSuccess == FAILURE_OP_CODE ) 
		{
			TEST_PRINT_RESULTS ( 
				testName , 
				ADD_FAILED_MESSAGE , 
				TEST_FAILED 
			);
			return TEST_FAILED;
		}
	}

	int count = ListCount ( list );
	if ( count != MAX_TEST_NUM_NODES ) 
	{
		TEST_PRINT_RESULTS ( 
			testName , 
			INCORRECT_COUNT_MESSAGE , 
			TEST_FAILED 
		);
		return TEST_FAILED;
	}

	TEST_PRINT_RESULTS ( 
		testName , 
		SUCCESSFUL_TEST_MESSAGE , 
		TEST_SUCCESS 
	);

	TEST_TEAR_DOWN ();
	return TEST_SUCCESS;
}

/* TEST SUITES */
void TEST_SUITE_STRESS_TESTING () {
	char testSuiteName [] = "TEST_SUITE_STRESS_TESTING";
	TEST_SUITE_PRINT_START ( testSuiteName ); 

	int numTestsPassed = 0;
	int numTestsFailed = 0;
	int numTestsUntestable = 0;
	
	int testResults1 = TEST_ALL_LISTS_USED ();
	if ( testResults1 == TEST_SUCCESS ) numTestsPassed++;
	else if ( testResults1 == TEST_FAILED ) numTestsFailed++;
	else if ( testResults1 == TEST_UNTESTABLE ) numTestsUntestable++;

	int testResults2 = TEST_ALL_NODES_USED_ONE_LIST ();
	if ( testResults2 == TEST_SUCCESS ) numTestsPassed++;
	else if ( testResults2 == TEST_FAILED ) numTestsFailed++;
	else if ( testResults2 == TEST_UNTESTABLE ) numTestsUntestable++;


	int testResults3 = TEST_MAX_LIST_TRIM_ALL_NODES ();
	if ( testResults3 == TEST_SUCCESS ) numTestsPassed++;
	else if ( testResults3 == TEST_FAILED ) numTestsFailed++;
	else if ( testResults3 == TEST_UNTESTABLE ) numTestsUntestable++;

	TEST_SUITE_PRINT_RESULTS ( 
		testSuiteName , 
		numTestsPassed , 
		numTestsFailed , 
		numTestsUntestable 
	);
}

void TEST_SUITE_ADD () 
{
	char testSuiteName [] = "TEST_SUITE_ADD";
	TEST_SUITE_PRINT_START ( testSuiteName ); 

	int numTestsPassed = 0;
	int numTestsFailed = 0;
	int numTestsUntestable = 0;

	int testResults1 = TEST_LIST_ADD ();
	if ( testResults1 == TEST_SUCCESS ) numTestsPassed++;
	else if ( testResults1 == TEST_FAILED ) numTestsFailed++;
	else if ( testResults1 == TEST_UNTESTABLE ) numTestsUntestable++;

	TEST_SUITE_PRINT_RESULTS ( 
		testSuiteName , 
		numTestsPassed , 
		numTestsFailed , 
		numTestsUntestable 
	);
}

void TEST_SUITE_FIRST_LAST () 
{
	char testSuiteName [] = "TEST_SUITE_FIRST_LAST";
	TEST_SUITE_PRINT_START ( testSuiteName ); 

	int numTestsPassed = 0;
	int numTestsFailed = 0;
	int numTestsUntestable = 0;
	
	int testResults1 = TEST_LIST_FIRST ();
	if ( testResults1 == TEST_SUCCESS ) numTestsPassed++;
	else if ( testResults1 == TEST_FAILED ) numTestsFailed++;
	else if ( testResults1 == TEST_UNTESTABLE ) numTestsUntestable++;

	int testResults2 = TEST_LIST_LAST ();
	if ( testResults2 == TEST_SUCCESS ) numTestsPassed++;
	else if ( testResults2 == TEST_FAILED ) numTestsFailed++;
	else if ( testResults2 == TEST_UNTESTABLE ) numTestsUntestable++;

	int testResults3 = TEST_LIST_FIRST_AND_LAST ();
	if ( testResults3 == TEST_SUCCESS ) numTestsPassed++;
	else if ( testResults3 == TEST_FAILED ) numTestsFailed++;
	else if ( testResults3 == TEST_UNTESTABLE ) numTestsUntestable++;

	TEST_SUITE_PRINT_RESULTS ( 
		testSuiteName , 
		numTestsPassed , 
		numTestsFailed , 
		numTestsUntestable 
	);
}

/* TEST DRIVER */
int main () 
{
	TEST_SUITE_ADD ();
	TEST_SUITE_FIRST_LAST ();
	TEST_SUITE_STRESS_TESTING ();

	return 0;
}