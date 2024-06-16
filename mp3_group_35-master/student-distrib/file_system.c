#include "file_system.h"
#include "lib.h"
#include "idt_assembly.h"
#include "System_Calls.h"

static int dir_entry = 0; 
/* Init File System - Initialize the file system
 * 
 * Initializes the file system, including the boot block, inodes, and data blocks.
 * Inputs: Pointer to the file system data
 * Outputs: None
 * Side Effects: Initializes the file system data structures
 * Coverage: File system initialization
 * Return: None
 */
void init_file_sys(uint32_t * myfileSystem){
    int i;
    my_boot_block = (boot_block_t*)(myfileSystem); 


    for(i=0; i<my_boot_block->dir_count; i++){ //this is to only create the number of inodes needed 
        dentry_t temp_dentry = my_boot_block->direntries[i];
        read_dentry_by_name((const uint8_t*)temp_dentry.filename, &temp_dentry);
        inodeArr[temp_dentry.inode_num] = (inode_t*)(myfileSystem + 1024  + (1024 * temp_dentry.inode_num));    
    }  

    //make new arr for data blocks 
    for(i = 0; i< my_boot_block->data_count; i++){
         data_block_arr[i] = (data_block_t*)(myfileSystem + (1024 * 65) + (i * 1024)); // Addr logic: start of the file system + all bytes for inodes + all bytes for all past blocks
         // L----> 1024 : the number of bytes per block / 4 ; 65: the nunmber of (inode blocks + boot block)  
    }
}

/* Read Directory Entry by Name - Search for a directory entry by name
 * 
 * Searches for a directory entry with the specified name and returns its information.
 * Inputs: fname - The name of the directory entry to search for
 *         dentry - A pointer to a dentry_t structure to store the found entry's information
 * Outputs: 0 if the entry is found, -1 if not found
 * Side Effects: None
 * Coverage: Directory entry lookup by name
  * Return: 0 if the entry is found, -1 if not found
 */
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry) {
    int i; 

    // int8_t bounded_fname[32];  // 32 characters plus null-terminator

    // for ( i = 0; i < 32; i++) {
    //     bounded_fname[i] = 0;
    // }
    int length = 0;

    if (fname == NULL)
        return -1;  // Not a valid string (null pointer)

    while (fname[length] != '\0' && length < 32) { // get the length of the fname
       length++;
    }


    // for ( i = 0; i < 32; i++) {
    //     bounded_fname[i] = fname[i];
    // }
    
///strcpy(dentry->filename, fname);
  // memcpy(dentry->filename,fname, length);

        for(i = 0; i < 63; i++){        ////WE NEED THE LENGTH AS A PARAMETER. 64: because 64 inodes 
            if (strncmp((const int8_t *)fname, (const int8_t *)my_boot_block->direntries[i].filename,(uint32_t)32) == 0) {//if 0, means the strings are the same 
                read_dentry_by_index(i, dentry);
                return 0; 
            }
            
        }
 return -1;
}

/* Read Directory Entry by Index - Get a directory entry by its index
 * 
 * Retrieves a directory entry by its index in the directory entry table.
 * Inputs: index - The index of the directory entry to retrieve
 *         dentry - A pointer to a dentry_t structure to store the entry's information
 * Outputs: 0 if the entry is valid and retrieved, -1 if the index is out of bounds
 * Side Effects: None
 * Coverage: Directory entry lookup by index
  * Return: 0 if the entry is valid and retrieved, -1 if the index is out of bounds
 */
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry){
    if (index < 0 || index > 63){ // 63 will go over the bound limit for the index
        return -1; 
    }
    strncpy((int8_t*)dentry->filename, (int8_t*)my_boot_block->direntries[index].filename,32);
    dentry->filetype = my_boot_block->direntries[index].filetype; // update the filetype 
    dentry->inode_num = my_boot_block->direntries[index].inode_num; // update the inode num
    return 0; 
}



/* Read Data - Read data from a file
 * 
 * Reads data from the specified inode starting at the given offset and stores it in a buffer.
 * Inputs: inode - The inode number of the file to read from
 *         offset - The offset within the file to start reading from
 *         buf - A buffer to store the read data
 *         length - The number of bytes to read
 * Outputs: The number of bytes successfully read, or -1 on failure
 * Side Effects: None
 * Coverage: File data reading
  * Return: The number of bytes successfully read, or -1 on failure
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    //offset = offset within file - where you want to start reading 
            // in bytes, can start within a data block 
    int i;
  
      //null check
    if(inode < 0 || inode >= 63){ //if outside of num of files (63 is always the limit)
        return -1; 
    }

    inode_t* tempInode = inodeArr[inode]; //create a temporary inode that holds just the current inode that we're getting data from 

//check this line
    if(length <0) {
        return -1;
    }

    if(offset < 0 ){
        return -1;
    }

   
//check this line

    if(tempInode->length - offset < length){ //if length (what we want to read) is greater than remaining bytes - if offset + length is greater than file length
        length = tempInode->length - offset; //make the length the max bytes that can be read with the given offset 
    }

      int ret_length = length;

    int iNode_block_idx = offset/4096; //index of the block data array from out inode struct. 4096: number of bytes per block

    int start_block_num = tempInode->data_block_num[iNode_block_idx]; //get data block located in the inode - nemory addr 


    int offset_into_start_block = offset % 4096; //getting the offset for the correct 4kb block of data in the file. 4096: number of bytes per block
    int temp_start_len = 0; // initially we wanna start at 0
    if(4096 - offset_into_start_block > length ){  //if the space between the end of the data block and the offset is larger than the area we want to read (read within a block)
        temp_start_len = length; //then set our temp length = to bytes we want to read, bc its within a block 
    }
    else{ //if the space between the end of the block and the offset is smaller than the total length we want to read 
        temp_start_len = 4096 - offset_into_start_block; //our temp length is just from the offset to the end of the block 
    }
    for( i = 0; i < temp_start_len; i++){
        buf[i] = (data_block_arr[start_block_num]->data_value[i + offset_into_start_block]);
    }
    length -= temp_start_len;
    iNode_block_idx+=1;

    int count = 0; 
    //offset is actually temp _start_length
    while(length/4096 != 0){
        int curr_block_num = tempInode->data_block_num[iNode_block_idx]; //get data block located in the inode - nemory addr 
       
        for(i=0; i<4096; i++){
            buf[i + temp_start_len + (4096*count)] = data_block_arr[curr_block_num]->data_value[i];
        }

        length -= 4096; // decrement the size of bytes for 1 block each iteration
        iNode_block_idx+=1;  // increment the block index by 1 each iteration 
        count ++; // increment the count each iteration
    }

    if(length != 0){
        int curr_block_num = tempInode->data_block_num[iNode_block_idx]; //get data block located in the inode - nemory addr 

        for(i=0; i<length; i++){
            buf[i + 4096 * count + temp_start_len] = data_block_arr[curr_block_num]->data_value[i]; // 4096 is the size of bytes per block
        }
        length = 0;
    }

    // if(tempInode->length == ret_length){
    //     return 0;
    // }

     if(offset >= tempInode->length){
        return 0;
    }


    return ret_length; 
}


/* Function to read data from a file
 * 
 * Reads data from the currently open file starting at the current offset.
 * Inputs: fd - File descriptor (not used in the function)
 *         buf - A buffer to store the read data
 *         nbytes - The number of bytes to read
 * Outputs: The number of bytes successfully read, or -1 on failure
 * Side Effects: Advances the global offset for the currently open file
  * Return: The number of bytes successfully read, or -1 on failure
 */
int32_t read_file(int32_t fd, void* buf, int32_t nbytes){
    //uint8_t* new_buf = (uint8_t*)buf;
    int temp_offset = pcb_array[target_pid]->fd_array[fd].file_position;
    pcb_array[target_pid]->fd_array[fd].file_position += nbytes;
     
    return read_data(pcb_array[target_pid]->fd_array[fd].inode, temp_offset, (uint8_t*)buf, nbytes);
}

/* Function to open a file
 * 
 * Finds and stores the directory entry for the given filename and resets the file offset.
 * Inputs: filename - The name of the file to open
 * Outputs: 0 on success
 * Side Effects: Resets the global offset for the currently open file
 * Coverage: File opening
 * Return: 0 on success
 */
int32_t open_file(const uint8_t* filename){
    // Find and store the directory entry for the given filename
    dentry_t file_dentry[1];
    int ret;
    ret = 0;
    ret = read_dentry_by_name(filename, file_dentry);
    global_offset = 0; // Reset the file offset to the beginning
    return ret; // Return success
}



/* Function to write to a file (Not implemented, returns -1)
 * 
 * This function is not implemented and always returns an error.
 * Inputs: fd - File descriptor (not used in the function)
 *         buf - A buffer containing data to write (not used)
 *         nbytes - The number of bytes to write (not used)
 * Outputs: -1 to indicate an error
 * Side Effects: None
 * Coverage: Not implemented
 * Return: -1 to indicate an error
 */
int32_t write_file(int32_t fd, const void* buf, int32_t nbytes){
    return -1; // Not implemented, return an error
}

/* Function to close a file
 * 
 * Resets the global offset for the currently open file.
 * Inputs: fd - File descriptor (not used in the function)
 * Outputs: 0 on success
 * Side Effects: Resets the global offset for the currently open file
 * Coverage: File closing
 * Return: 0 on success
 */
int32_t close_file(int32_t fd){
    global_offset = 0; // Reset the file offset
    return 0; // Return success
}



/* Function to open a directory
 * 
 * Resets the index offset for reading directory entries and finds and stores the directory entry for the given filename.
 * Inputs: filename - The name of the directory to open
 * Outputs: 0 on success
 * Side Effects: Resets the index offset for reading directory entries
 * Coverage: Directory opening
 * Return: 0 on success
 */
int32_t open_dir(const uint8_t* filename){
    //index_offset = 0; // Reset the index offset for reading directory entries
    int ret;
    ret = 0;
    ret = read_dentry_by_name(filename, dir_dentry); // Find and store the directory entry for the given filename
    return ret; // Return success
}


/* Function to read directory entries
 * 
 * Reads and prints directory entries starting from the current index offset.
 * Inputs: fd - File descriptor (not used in the function)
 *         buf - A buffer to store the directory entry data
 *         nbytes - The maximum number of bytes to read (not used)
 * Outputs: The size of the directory entry read, or 0 if no more entries are available
 * Side Effects: Advances the index offset for reading directory entries
 * Coverage: Directory entry reading
 * Return: The size of the directory entry read, or 0 if no more entries are available
 */
int32_t read_dir(int32_t fd, void* buf, int32_t nbytes) {
    int i, k;
    int l;
    dentry_t new_dentry[1];

    // A check to include an unused variable in the code and avoid warnings
    if (new_dentry->inode_num != 819289273) {
        l = 1;
    }

    // Cast the buffer to uint8_t for byte-wise manipulation
    uint8_t* buffy = (uint8_t*)buf;

    // Temporary buffer to store file names
    uint8_t file_name[32];

    // Initialize the file name buffer
    for (k = 0; k < 32; k++) {
        file_name[k] = 0;
    }

    // Copy the file name from the directory entry to the temporary buffer
    strncpy((int8_t*)file_name, (int8_t*)my_boot_block->direntries[dir_entry].filename, 32);

    // Copy the file name from the temporary buffer to the output buffer
    for (i = 0; i < 32; i++) {
        buffy[i] = file_name[i];
    }

    // Add a newline character after the file name
    file_name[i] = '\n';

    // Increment the directory entry index
    dir_entry += 1;

    // Check if there are more directory entries to read
    if (dir_entry <= 17) {
        return i; // Return the number of bytes copied into the buffer
    } 
    else {
        // Reset the directory entry index and signal the end of entries
        dir_entry = 0;
        return 0; // Return 0 to indicate the end of directory entries
    }
}



/* Function to close a directory
 * 
 * Resets the index offset for reading directory entries.
 * Inputs: fd - File descriptor (not used in the function)
 * Outputs: 0 on success
 * Side Effects: Resets the index offset for reading directory entries
 * Coverage: Directory closing
 * Return: 0 on success
 */
int32_t close_dir(int32_t fd){
   
    return 0; // Return success
}


/* Function to write to a directory (Not implemented, returns -1)
 * 
 * This function is not implemented and always returns an error.
 * Inputs: fd - File descriptor (not used in the function)
 *         buf - A buffer containing data to write (not used)
 *         nbytes - The number of bytes to write (not used)
 * Outputs: -1 to indicate an error
 * Side Effects: None
 * Coverage: Not implemented
 * Return: -1 to indicate an error
 */
int32_t write_dir(int32_t fd, const void* buf, int32_t nbytes){
    return -1; // Not implemented, return an error
}



/* Function to initialize a file descriptor array
 * Initializes the file descriptor array with null values for file descriptors 2 to 7, and sets up file descriptors 0 and 1 with the correct jump tables.
 * Inputs:
 *   file_descriptor_array - Array of file descriptors
 * Outputs: Initializes the file descriptor array
 * Return: None
 */
void init_file_descriptor(file_descriptor_struct_t file_descriptor_array[8]) {
    
    int i;
    //for (i = 0; i < 10; i++) {fd->jmp_table[i] = sys_call_table[i];}
    //printf("entering init function");
    file_descriptor_struct_t struct_0; 
    file_descriptor_struct_t struct_1; 
    for(i = 2; i<8; i++){
        file_descriptor_array[i].flags = 0;
        //printf("i: %d\n", i );
    }
    //printf("finished for loop setting nulls \n");


    //initializing 0 and 1 with correct jump table
    //idk if this syntax is right but the idea is map functions into array of jump table one by one
    for (i = 0; i< 4; i++){ // iterate over all 4: open,close,read,wrirte
        struct_0.jmp_table[i] = terminal_jump_table[i];
        struct_1.jmp_table[i] =  terminal_jump_table[i];
        //printf("i: %d\n", i );
    }
    // struct_0.jmp_table[0] = terminal_jump_table[0]; 
    // struct_1.jmp_table[1] = terminal_jump_table[1]; 

    //printf("finished second for loop");
    struct_0.inode = 0;
    struct_1.inode = 0;

    struct_0.file_position = 0; 
    struct_1.file_position = 0; 

    //in use
    struct_0.flags = 1; 
    struct_1.flags = 1; 
//printf("finished temp structs");

file_descriptor_array[0] = struct_0; 
file_descriptor_array[1] = struct_1; 
//printf("finished init");
}


/* Function to add a file descriptor table entry
 * Adds a file descriptor table entry for the specified file descriptor (fd) with the given filename. Determines the file type, sets up the appropriate jump table, and initializes other fields.
 * Inputs:
 *   fd - File descriptor index
 *   filename - Name of the file
 *   file_descriptor_array - Array of file descriptors
 * Outputs: Initializes the file descriptor entry in the file descriptor array
 * Return: None
 */
void add_file_descriptor_table_entry(int fd, uint8_t* filename, file_descriptor_struct_t file_descriptor_array[8]){
    //i think this will have to be called from open 

    //file types: 0 is RTC, 1 is directory, 2 is regular file
    int i=0;
    dentry_t temp_dentry; 
    file_descriptor_struct_t temp_struct; 
    temp_struct.inode = 0; //initialized to zero, only non zero if a file
    read_dentry_by_name(filename, &temp_dentry); 
    if (temp_dentry.filetype == 0){
        
        for (i = 0; i< 4; i++){ //iterate over each value in the jump table: 1 for open,close,read,write
            temp_struct.jmp_table[i] = rtc_jump_table[i];
        }
    }


    if (temp_dentry.filetype == 1){
        for (i = 0; i< 4; i++){
            temp_struct.jmp_table[i] = dir_jump_table[i];
        }
    }

    if (temp_dentry.filetype == 2){
        for (i = 0; i< 4; i++){
            temp_struct.jmp_table[i] = file_jump_table[i];
        }
        temp_struct.inode = temp_dentry.inode_num;//reset inode num for file
    }

    temp_struct.flags = 1; //always at 1? idrk 
    temp_struct.file_position = 0; //init to zero, each read will change its value
    
    file_descriptor_array[fd] = temp_struct; 
   return;

}


int32_t (*file_jump_table[4]) () = {read_file, write_file, open_file, close_file};

int32_t (*dir_jump_table[4]) () = {read_dir, write_dir, open_dir, close_dir};
