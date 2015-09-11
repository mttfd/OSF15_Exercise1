#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


#include <limits.h>


#include "matrix.h"


#define MAX_CMD_COUNT 50

/*protected functions*/
void load_matrix (Matrix_t* m, unsigned int* data);

/*
 * PURPOSE: instantiates a new matrix with the passed name, rows, cols
 * INPUTS:
 *	name the name of the matrix limited to 50 characters
 *  rows the number of rows the matrix
 *  cols the number of cols the matrix
 * RETURN:
 *  If no errors occurred during instantiation then true
 *  else false for an error in the process.
 *
 **/

bool create_matrix (Matrix_t** new_matrix, const char* name, const unsigned int rows,
						const unsigned int cols) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(new_matrix == NULL || name == NULL) {
		printf("NULL pointers get passed in.\n");
		return false;
	}


	if( rows < 0 || cols < 0 || rows > UINT_MAX || cols > UINT_MAX) {
		printf("rows or cols is not in range\n");
		return false;
	}

	*new_matrix = calloc(1,sizeof(Matrix_t));
	if (!(*new_matrix)) {
		return false;
	}
	(*new_matrix)->data = calloc(rows * cols,sizeof(unsigned int));
	if (!(*new_matrix)->data) {
		return false;
	}
	(*new_matrix)->rows = rows;
	(*new_matrix)->cols = cols;
	unsigned int len = strlen(name) + 1;
	if (len > MATRIX_NAME_LEN) {
		return false;
	}
	strncpy((*new_matrix)->name,name,len);
	return true;

}

	//TODO FUNCTION COMMENT
/*
 * PURPOSE: gets a matrix and destroys it
 * INPUTS:
 *	m the matrix to be destroyed
 * RETURN:
 *  no returned value
 *
 **/
void destroy_matrix (Matrix_t** m) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(m == NULL || *m == NULL) {
		printf("NULL pointers.\n");
		return;
	}

	free((*m)->data);
	free(*m);
	*m = NULL;
}



	//TODO FUNCTION COMMENT
/*
 * PURPOSE: compare two passed matrices
 * INPUTS:
 *	a one matrix to be compared
 *  b the other matrix to be compared
 * RETURN:
 *  If no errors occurred and two matrices are equal then true
 *  else false
 *
 **/
bool equal_matrices (Matrix_t* a, Matrix_t* b) {

	//TODO ERROR CHECK INCOMING PARAMETERS

	if (!a || !b || !a->data || !b->data) {
		return false;
	}

	int result = memcmp(a->data,b->data, sizeof(unsigned int) * a->rows * a->cols);
	if (result == 0) {
		return true;
	}
	return false;
}

	//TODO FUNCTION COMMENT
/*
 * PURPOSE: duplicates a matrix
 * INPUTS:
 *	src the source matrix to be duplicated
 * RETURN:
 *  If no errors occurred and duplicated successfully then true
 *  else false.
 *
 **/
bool duplicate_matrix (Matrix_t* src, Matrix_t* dest) {


	//TODO ERROR CHECK INCOMING PARAMETERS
	if(src == NULL || dest == NULL) {
		printf("NULL pointers get passed in.\n");
		return false;
	}

	if(src->cols < 0 || src->rows < 0) {
		printf("invalid source matrix.\n");
		return false;
	}


	if (!src) {
		return false;
	}
	/*
	 * copy over data
	 */
	unsigned int bytesToCopy = sizeof(unsigned int) * src->rows * src->cols;
	memcpy(dest->data,src->data, bytesToCopy);
	return equal_matrices (src,dest);
}

	//TODO FUNCTION COMMENT
/*
 * PURPOSE: shift passed bits in a passed direction for every entries in a passed matrix
 * INPUTS:
 *	a the source matrix to be shifted
 *  l direction that specifies left shift or right shift
 * RETURN:
 *  If no errors occurred and bit shifted successfully then true
 *  else false.
 *
 **/
bool bitwise_shift_matrix (Matrix_t* a, char direction, unsigned int shift) {

	//TODO ERROR CHECK INCOMING PARAMETERS

	if(direction != 'l' && direction != 'r') {
		printf("invalid direction.\n");
		return false;
	}

	if(shift < 0 || shift > UINT_MAX) {
		printf("shift is not in range.\n");
		return false;
	}

	if (!a) {
		printf("NULL pointer gets passed in.\n");
		return false;
	}

	if (direction == 'l') {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] << shift;
			}
		}

	}
	else {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] >> shift;
			}
		}
	}

	return true;
}

	//TODO FUNCTION COMMENT
/*
 * PURPOSE: adds two passed matrices and stores the result matrix into third parameter
 * INPUTS:
 *	a a matrix to be added
 *  b the other matrix
 * RETURN:
 *  If no errors occurred and added successfully then true
 *  else false.
 *
 **/
bool add_matrices (Matrix_t* a, Matrix_t* b, Matrix_t* c) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(a == NULL || b == NULL || c == NULL) {
		printf("NULL pointers get passed in.\n");
		return false;
	}

	if(!a->rows || !a->cols || !b->rows || !b->cols) {
		printf("invalid source matrices.\n");
		return false;
	}

	if (a->rows != b->rows && a->cols != b->cols) {
		return false;
	}

	for (int i = 0; i < a->rows; ++i) {
		for (int j = 0; j < b->cols; ++j) {
			c->data[i * a->cols +j] = a->data[i * a->cols + j] + b->data[i * a->cols + j];
		}
	}
	return true;
}

	//TODO FUNCTION COMMENT
/*
 * PURPOSE: display a matrix
 * INPUTS:
 *	m the  matrix to be displayed
 * RETURN:
 *  no returned value
 *
 **/
void display_matrix (Matrix_t* m) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(m == NULL) {
		printf("NULL pointer get passed in.\n");
		return;
	}

	if(!m->rows || !m->cols || !m->name || !m->data) {
		printf("invalid matrix.\n");
		return;
	}

	printf("\nMatrix Contents (%s):\n", m->name);
	printf("DIM = (%u,%u)\n", m->rows, m->cols);
	for (int i = 0; i < m->rows; ++i) {
		for (int j = 0; j < m->cols; ++j) {
			printf("%u ", m->data[i * m->cols + j]);
		}
		printf("\n");
	}
	printf("\n");

}

	//TODO FUNCTION COMMENT
/*
 * PURPOSE: read a matrix from a file
 * INPUTS:
 *	matrix_input_filename filename of the file storing a matrix
 * RETURN:
 *  If no errors occurred and read successfully then true
 *  else false.
 *
 **/
bool read_matrix (const char* matrix_input_filename, Matrix_t** m) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(matrix_input_filename == NULL) {
		printf("NULL pointer gets passed in.\n");
		return false;
	}

	int fd = open(matrix_input_filename,O_RDONLY);
	if (fd < 0) {
		printf("FAILED TO OPEN FOR READING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}

	/*read the wrote dimensions and name length*/
	unsigned int name_len = 0;
	unsigned int rows = 0;
	unsigned int cols = 0;

	if (read(fd,&name_len,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READING FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}
	char name_buffer[50];
	if (read (fd,name_buffer,sizeof(char) * name_len) != sizeof(char) * name_len) {
		printf("FAILED TO READ MATRIX NAME\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	if (read (fd,&rows, sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX ROW SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	if (read(fd,&cols,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX COLUMN SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	unsigned int numberOfDataBytes = rows * cols * sizeof(unsigned int);
	unsigned int *data = calloc(rows * cols, sizeof(unsigned int));
	if (read(fd,data,numberOfDataBytes) != numberOfDataBytes) {
		printf("FAILED TO READ MATRIX DATA\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	if (!create_matrix(m,name_buffer,rows,cols)) {
		return false;
	}

	load_matrix(*m,data);
	free(data);
	if (close(fd)) {
		return false;

	}
	return true;
}

	//TODO FUNCTION COMMENT
/*
 * PURPOSE: write a matrix into a file
 * INPUTS:
 *	m the source matrix
 * RETURN:
 *  If no errors occurred and wrote successfully then true
 *  else false.
 *
 **/
bool write_matrix (const char* matrix_output_filename, Matrix_t* m) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(m == NULL) {
		printf("NULL pointer get passed in.\n");
		return false;
	}

	int fd = open (matrix_output_filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	/* ERROR HANDLING USING errorno*/
	if (fd < 0) {
		printf("FAILED TO CREATE/OPEN FILE FOR WRITING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXISTS\n");
		}
		return false;
	}
	/* Calculate the needed buffer for our matrix */
	unsigned int name_len = strlen(m->name) + 1;
	unsigned int numberOfBytes = sizeof(unsigned int) + (sizeof(unsigned int)  * 2) + name_len + sizeof(unsigned int) * m->rows * m->cols + 1;
	/* Allocate the output_buffer in bytes
	 * IMPORTANT TO UNDERSTAND THIS WAY OF MOVING MEMORY
	 */
	unsigned char* output_buffer = calloc(numberOfBytes,sizeof(unsigned char));
	unsigned int offset = 0;
	memcpy(&output_buffer[offset], &name_len, sizeof(unsigned int)); // IMPORTANT C FUNCTION TO KNOW
	offset += sizeof(unsigned int);
	memcpy(&output_buffer[offset], m->name,name_len);
	offset += name_len;
	memcpy(&output_buffer[offset],&m->rows,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy(&output_buffer[offset],&m->cols,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy (&output_buffer[offset],m->data,m->rows * m->cols * sizeof(unsigned int));
	offset += (m->rows * m->cols * sizeof(unsigned int));
	output_buffer[numberOfBytes - 1] = EOF;

	if (write(fd,output_buffer,numberOfBytes) != numberOfBytes) {
		printf("FAILED TO WRITE MATRIX TO FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}

	if (close(fd)) {
		return false;
	}
	free(output_buffer);

	return true;
}

	//TODO FUNCTION COMMENT
/*
 * PURPOSE: create a matrix with random entries in a given range
 * INPUTS:
 *	start_range lower bound of the random number range(closed)
 *	end_range upper bound of the random number range(not closed)
 * RETURN:
 *  If no errors occurred and generated successfully then true
 *  else false.
 *
 **/
bool random_matrix(Matrix_t* m, unsigned int start_range, unsigned int end_range) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(m == NULL) {
		printf("NULL pointer get passed in.\n");
		return false;
	}

	if(!m -> rows || !m->cols || !m->data) {
		printf("invalid matrix.\n");
		return false;
	}

	if(start_range < 0 || end_range > UINT_MAX || end_range < start_range) {
		printf("invalid range.\n");
		return false;
	}

	for (unsigned int i = 0; i < m->rows; ++i) {
		for (unsigned int j = 0; j < m->cols; ++j) {
			m->data[i * m->cols + j] = rand() % (end_range + 1 - start_range) + start_range;
		}
	}
	return true;
}

/*Protected Functions in C*/

	//TODO FUNCTION COMMENT
/*
 * PURPOSE: load data to a existing matrix
 * INPUTS:
 *	data given matrix data
 * RETURN:
 * 	no returned value
 *
 **/
void load_matrix (Matrix_t* m, unsigned int* data) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(m == NULL || data == NULL) {
		printf("NULL pointers get passed in.\n");
		return;
	}


	if(!m -> data || !m->rows || !m->cols) {
		printf("invalid matrix.\n");
		return;
	}



	memcpy(m->data,data,m->rows * m->cols * sizeof(unsigned int));
}

	//TODO FUNCTION COMMENT
/*
 * PURPOSE: add a matrix to the array mats; if there is a matrix in current position, then replace it by the passed matrix
 * INPUTS:
 *	mats the matrix array
 *	new_matrix the matrix to be stored
 *	num_mats max length of the matrix array
 * RETURN:
 * 	the index where the matrix gets stored in the array
 *
 **/
unsigned int add_matrix_to_array (Matrix_t** mats, Matrix_t* new_matrix, unsigned int num_mats) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(mats == NULL || new_matrix == NULL) {
		printf("NULL pointers get passed in.\n");
		return -1;
	}

	if(num_mats < 0 || num_mats > UINT_MAX) {
		printf("number of matrices is not in range\n");
		return -1;
	}


	static long int current_position = 0;
	const long int pos = current_position % num_mats;
	if ( mats[pos] ) {
		destroy_matrix(&mats[pos]);
		printf("some matrix has been destroyed!\n");
	}
	mats[pos] = new_matrix;
	current_position++;
	return pos;
}
