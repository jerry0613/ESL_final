#include "string.h"
#include "cassert"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "main.h"

uint32_t print_sem[PROCESSORS];
uint32_t write_sem;
uint32_t read_sem;
uint32_t barrier_counter = 0; 
uint32_t barrier_lock; 
uint32_t barrier_sem; 
FILE* file;

int main(unsigned hart_id) {
	int theta = 55;
	float R[3] = {0, 1, 0};
	float T[3] = {0.5, 0, 0};
	float V[36][3] = {
		{-0.5f, -0.5f, -0.5f},  
         {0.5f, -0.5f, -0.5f},  
         {0.5f,  0.5f, -0.5f},  
         {0.5f,  0.5f, -0.5f}, 
        {-0.5f,  0.5f, -0.5f},  
        {-0.5f, -0.5f, -0.5f},  

        {-0.5f, -0.5f,  0.5f},  
         {0.5f, -0.5f,  0.5f},  
         {0.5f,  0.5f,  0.5f}, 
         {0.5f,  0.5f,  0.5f}, 
        {-0.5f,  0.5f,  0.5f},  
        {-0.5f, -0.5f,  0.5f},  

        {-0.5f,  0.5f,  0.5f},  
        {-0.5f,  0.5f, -0.5f},  
        {-0.5f, -0.5f, -0.5f}, 
        {-0.5f, -0.5f, -0.5f}, 
        {-0.5f, -0.5f,  0.5f},  
        {-0.5f,  0.5f,  0.5f}, 

         {0.5f,  0.5f,  0.5f},  
         {0.5f,  0.5f, -0.5f},  
         {0.5f, -0.5f, -0.5f},  
         {0.5f, -0.5f, -0.5f}, 
         {0.5f, -0.5f,  0.5f},  
         {0.5f,  0.5f,  0.5f},  

        {-0.5f, -0.5f, -0.5f}, 
         {0.5f, -0.5f, -0.5f}, 
         {0.5f, -0.5f,  0.5f}, 
         {0.5f, -0.5f,  0.5f}, 
        {-0.5f, -0.5f,  0.5f}, 
        {-0.5f, -0.5f, -0.5f}, 

        {-0.5f,  0.5f, -0.5f},  
         {0.5f,  0.5f, -0.5f},  
         {0.5f,  0.5f,  0.5f},  
         {0.5f,  0.5f,  0.5f}, 
        {-0.5f,  0.5f,  0.5f},  
        {-0.5f,  0.5f, -0.5f} 
	};
	int tmp;	
	float x, y, z;
	unsigned char data[6] = {0};
	char tmp_x[256];
	char tmp_y[256];
	char tmp_z[256];
	char out[256];
	// open file
	if (hart_id == 0)
		file = fopen("output_vertex.txt", "w");

	// initialize semaphore
	if (hart_id == 0) {
		sem_init(&write_sem, 0);
		sem_init(&read_sem, 0);
		sem_init(&barrier_lock, 1);
		sem_init(&barrier_sem, 0); //lock all cores initially
		for (int i = 0; i < PROCESSORS; i++)
			sem_init(&print_sem[i], 0);
	}
	// input theta
	data[0] = theta;
	if (hart_id == 0) {
		write_data_to_ACC(THETA_ADDR0, data, 1);
		sem_post(&write_sem);
	} else {
		sem_wait(&write_sem);
		write_data_to_ACC(THETA_ADDR1, data, 1);
	}
	barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);
	
	// input translation
	tmp = (int)(T[0]*8) + ((int)(T[1]*8) << 5) + ((int)(T[2]*8) << 10);
	data[0] = tmp;
	data[1] = tmp >> 8;
	if (hart_id == 0) {
		write_data_to_ACC(T_ADDR0, data, 2);
		sem_post(&write_sem);
	} else {
		sem_wait(&write_sem);
		write_data_to_ACC(T_ADDR1, data, 2);
	}
	barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);

	// normalize R
	float len;
	len = sqrt(R[0]*R[0] + R[1]*R[1] + R[2]*R[2]);
	tmp = (int)(R[0] * (1 << 7) / len) + ((int)(R[1] * (1 << 7) / len) << 8) + ((int)(R[2] * (1 << 7) / len) << 16);
	data[0] = tmp;
	data[1] = tmp >> 8;
	data[2] = tmp >> 16;

	if (hart_id == 0) {
		write_data_to_ACC(R_ADDR0, data, 3);
		sem_post(&write_sem);
	} else {
		sem_wait(&write_sem);
		write_data_to_ACC(R_ADDR1, data, 3);
	}
	barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);

	int i;
	// input and output x, y, z
	for (int j = 0; j < 18; j++) {
		if (hart_id == 0)
			i = 2*j;
		else
			i = 2*j + 1;
		
		tmp = (int)(V[i][0] * 16384);
		data[0] = tmp;
		data[1] = tmp >> 8;
		if (hart_id == 0) {
			write_data_to_ACC(IN_X_ADDR0, data, 2);
			sem_post(&write_sem);
		} else {
			sem_wait(&write_sem);
			write_data_to_ACC(IN_X_ADDR1, data, 2);
		}
		barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);
		tmp = (int)(V[i][1] * 16384);
		data[0] = tmp;
		data[1] = tmp >> 8;
		if (hart_id == 0) {
			write_data_to_ACC(IN_Y_ADDR0, data, 2);
			sem_post(&write_sem);
		} else {
			sem_wait(&write_sem);
			write_data_to_ACC(IN_Y_ADDR1, data, 2);
		}
		barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);
		tmp = (int)(V[i][2] * (1 << 12));
		data[0] = tmp;
		data[1] = tmp >> 8;
		if (hart_id == 0) {
			write_data_to_ACC(IN_Z_ADDR0, data, 2);
			sem_post(&write_sem);
		} else {
			sem_wait(&write_sem);
			write_data_to_ACC(IN_Z_ADDR1, data, 2);
		}
		barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);
		if (hart_id == 0) {
			read_data_from_ACC(OUT_X_ADDR0, data, 2);
			sem_post(&read_sem);
		} else {
			sem_wait(&read_sem);
			read_data_from_ACC(OUT_X_ADDR1, data, 2);
		}
		barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);
		tmp = (data[1] << 8) + data[0];
		if ((data[1] >> 7) == 1)
			tmp = tmp + (65535 << 16);
		x = (float)(tmp) / 16384;

		if (hart_id == 0) {
			read_data_from_ACC(OUT_Y_ADDR0, data, 2);
			sem_post(&read_sem);
		} else {
			sem_wait(&read_sem);
			read_data_from_ACC(OUT_Y_ADDR1, data, 2);
		}
		barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);
		tmp = (data[1] << 8) + data[0];
		if ((data[1] >> 7) == 1)
			tmp = tmp + (65535 << 16);
		y = (float)(tmp) / 16384;

		if (hart_id == 0) {
			read_data_from_ACC(OUT_Z_ADDR0, data, 2);
			sem_post(&read_sem);
		} else {
			sem_wait(&read_sem);
			read_data_from_ACC(OUT_Z_ADDR1, data, 2);
		}
		barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);
		tmp = (data[1] << 8) + data[0];
		if ((data[1] >> 7) == 1)
			tmp = tmp + (65535 << 16);
		z = (float)(tmp) / (1 << 12);

		if (hart_id == 0) {
			sprintfloat(tmp_x, x);
			sprintfloat(tmp_y, y);
			sprintfloat(tmp_z, z);
			sprintf(out, "[%s, %s, %s],\n", tmp_x, tmp_y, tmp_z);
			fputs(out, file);
			sem_post(&print_sem[1]);
		} else {
			sem_wait(&print_sem[1]); 
			sprintfloat(tmp_x, x);
			sprintfloat(tmp_y, y);
			sprintfloat(tmp_z, z);
			sprintf(out, "[%s, %s, %s],\n", tmp_x, tmp_y, tmp_z);
			fputs(out, file);
		}
	}

	barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);
	if (hart_id == 0)
		fclose(file);
	return 0;
}
