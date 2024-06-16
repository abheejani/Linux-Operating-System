#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "file_system.h"
#include "System_Calls.h"


#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15"); //change this everytime you wanna test 
}

/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}
	return PASS; 
}

// add more tests here

int invalidPageAccess(){
    int i;
    char * ptr;
    int value;
    for(i = 0x400000; i < 0x800000; i++){
        ptr = (char *)i;
        value = *ptr;
    }
    return PASS;
}

int invalidMaxTest(){
    int i = 0x810000;
    char* ptr;
    int value;
    ptr = (char *)i;
    value = *ptr;
    return FAIL;
}

int invalidMinTest(){
    int i = 0x390000;
    char* ptr;
    int value;
    ptr = (char *)i;
    value = *ptr;
    return FAIL;
}

int invalidVidMemTest(){
    int i;
    char* ptr;
    int value;
    for(i = 0xb8000; i < 0xb9000; i++){
        ptr = (char *)i;
        value = *ptr;
    }
    return PASS;
}

 


/* Checkpoint 2 tests */
void launch_tests(){
    //     uint8_t pointerr[0x8400000 - 0x8048000]; 
    // uint8_t buf[25]; 
    //   read_data(55, 4096, buf, 25);
    
	//MAKE FLAG = 1 FOR RTC OR 0 FOR KEYBOARD INTERRUPT 
	flag = 0;
    // uint8_t buf_name[32];
    // uint8_t dir_name[32];
    // uint8_t boofer[3000];


    //;


        // First test for file system (frame0.txt)
    // strcpy((int8_t*)buf_name, "frame0.txt");  // Copy "frame0.txt" into buf_name
    // open_file(buf_name);
    // read_file(0, boofer, 20000);
    // close_file(NULL);


    // strcpy((int8_t*)buf_name, ".");  // Copy "frame0.txt" into buf_name
    // open_syscall(buf_name);
    // read_syscall(0, boofer, 20000);
    // read_file(0, boofer, 20000);
    // close_file(NULL);

    // while(boofer[i] != 0 || boofer[i] != '\0'){
    //     printf("%c");
    //     i++;
    // }



    //     // Second test for file system (cat)
    // strcpy((int8_t*)buf_name, "ls");  // Copy "ls" into buf_name
    // open_file(buf_name);
    // read_file(NULL, boofer, 37000);
    // close_file(NULL);


          // Second test for file system (cat)
    // strcpy((int8_t*)buf_name, "ls");  // Copy "ls" into buf_name
    // open_file(buf_name);
    // read_file(NULL, boofer, 600000); //1068 and above is fucked
    // close_file(NULL);

    //read_data(5, 0, buf, nbytes);




    //     // Third test for file system w/ consecutive read
    // strcpy((int8_t*)buf_name, "frame1.txt");
    // open_file(buf_name);
    // int i;
    // for(i = 0; i < 100000000; i++){
    //     read_file(NULL, boofer, 1);
    // }
    // close_file(NULL);





    // strcpy((int8_t*)buf_name, "verylargetextwithverylongname.txt");
    //open_syscall("helloo");
    // read_file(NULL,boofer,10);
    // read_file(NULL,boofer,10);
    // read_file(NULL,boofer,10);
    // read_file(NULL,boofer,10);
    // read_file(NULL,boofer,10);
    // close_file(NULL);





//     pcb_array[0]->pid = 0; 
    
//     pcb_array[0]->state = 1;

//     init_file_descriptor(pcb_array[0]->fd_array);



//     strcpy((int8_t*)dir_name, ".");
//     uint8_t dir_boofer[100000];
//     int i;
//     open_syscall(dir_name);
//     for(i =0; i< 10000;i++){
        
//         read_syscall(fd_index, dir_boofer, 1);
//         write_syscall(fd_index, dir_boofer, 1);
//     }
//    close_syscall(NULL);

    // strcpy((int8_t*)dir_name, ".");
    // uint8_t dir_boofer[999];
    // int i;
    // open_dir(dir_name);
    // for(i =0; i< 1068;i++){
        
    //     read_dir(NULL, dir_boofer, 1);
    // }
    // close_dir(NULL);








    // strcpy((int8_t*)dir_name, ".");
    // uint8_t dir_boofer[999];

    // open_dir(dir_name);
    // read_dir(NULL, dir_boofer, 0);
    // read_dir(NULL, dir_boofer, 0);
    // close_dir(NULL);
    // read_dir(NULL, dir_boofer, 0);
    // read_dir(NULL, dir_boofer, 0);
    // close_dir(NULL);




//    init_file_sys(myfileSystem, fileSysEnd);
//    inode_passed_in = 5;
//    offset_passed_in = 0;
//    length_passed_in_read_file = 999;
   //read_file(NULL, boofer, length_passed_in_read_file);
    // read_data(25, 0, boofer, 1000000);

    // printf("return value: %d", ret);




	// // Trigger exception 0
	// asm volatile("int $0");

	// // Trigger exception 1
	// asm volatile("int $1");

    // // Trigger exception 2
    // asm volatile("int $2");

    // // Trigger exception 3
    // asm volatile("int $3");

    // // Trigger exception 4
    // asm volatile("int $4");

	// // Trigger exception 5
	// asm volatile("int $5");

    // // Trigger exception 6
    // asm volatile("int $6");

    // // Trigger exception 7
    // asm volatile("int $7");

    // // Trigger exception 8
    // asm volatile("int $8");

    // // Trigger exception 10
    // asm volatile("int $10");

    // // Trigger exception 11
    // asm volatile("int $11");

    // // Trigger exception 12
    // asm volatile("int $12");

    // // Trigger exception 13
    // asm volatile("int $13");

    // // Trigger exception 14
    // asm volatile("int $14");

    // // Trigger exception 16
    // asm volatile("int $16");

    // // Trigger exception 17
    // asm volatile("int $17");

    // // Trigger exception 18
    // asm volatile("int $18");

    // // Trigger exception 19
    // asm volatile("int $19");

    // // Trigger exception 20
    // asm volatile("int $20");

    // // Trigger exception 21
    // asm volatile("int $21");

    // // Trigger exception 28
    // asm volatile("int $28");

    // // Trigger exception 29
    // asm volatile("int $29");

    // // Trigger exception 30
    // asm volatile("int $30");
	
	// // System Call
//    asm volatile("int $128");

//    printf("returned from sys call");

   // open_syscall("ls");
    //read_file("ls");
    


    // invalidPageAccess();
    // invalidMaxTest();
    // invalidMinTest();
    //invalidVidMemTest();

	//Overflow excpetion 
	//asm volatile("int $4");
	
	// Trigger a Non-Maskable Interrupt (NMI)
    //asm volatile("int $2");

	//Debug exception
	//asm volatile("int $1");

	//Invalid Opcode Exception
	//asm volatile("ud2");  // Execute an undefined instruction (UD2)

	//breakpoint exception
	//asm volatile("int $3");
	
	//div 0 exception
	//int a = 1/0; 
	
	// Test for page fault
	// int* c = 0; 
	// int b = *c; 

}
