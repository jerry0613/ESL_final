int sem_init (uint32_t *__sem, uint32_t count) __THROW
{
  *__sem=count;
  return 0;
}

int sem_wait (uint32_t *__sem) __THROW
{
  uint32_t value, success; //RV32A
  __asm__ __volatile__("\
L%=:\n\t\
     lr.w %[value],(%[__sem])            # load reserved\n\t\
     beqz %[value],L%=                   # if zero, try again\n\t\
     addi %[value],%[value],-1           # value --\n\t\
     sc.w %[success],%[value],(%[__sem]) # store conditionally\n\t\
     bnez %[success], L%=                # if the store failed, try again\n\t\
"
    : [value] "=r"(value), [success]"=r"(success)
    : [__sem] "r"(__sem)
    : "memory");
  return 0;
}

int sem_post (uint32_t *__sem) __THROW
{
  uint32_t value, success; //RV32A
  __asm__ __volatile__("\
L%=:\n\t\
     lr.w %[value],(%[__sem])            # load reserved\n\t\
     addi %[value],%[value], 1           # value ++\n\t\
     sc.w %[success],%[value],(%[__sem]) # store conditionally\n\t\
     bnez %[success], L%=                # if the store failed, try again\n\t\
"
    : [value] "=r"(value), [success]"=r"(success)
    : [__sem] "r"(__sem)
    : "memory");
  return 0;
}

int barrier(uint32_t *__sem, uint32_t *__lock, uint32_t *counter, uint32_t thread_count) {
	sem_wait(__lock);
	if (*counter == thread_count - 1) { //all finished
		*counter = 0;
		sem_post(__lock);
		for (int j = 0; j < thread_count - 1; ++j) sem_post(__sem);
	} else {
		(*counter)++;
		sem_post(__lock);
		sem_wait(__sem);
	}
	return 0;
}

#define PROCESSORS 2

// address
static char* const THETA_ADDR0 = reinterpret_cast<char* const>(0x73000000);
static char* const     R_ADDR0 = reinterpret_cast<char* const>(0x73000004);
static char* const     T_ADDR0 = reinterpret_cast<char* const>(0x73000008);
static char* const  IN_X_ADDR0 = reinterpret_cast<char* const>(0x7300000c);
static char* const  IN_Y_ADDR0 = reinterpret_cast<char* const>(0x73000010);
static char* const  IN_Z_ADDR0 = reinterpret_cast<char* const>(0x73000014);
static char* const OUT_X_ADDR0 = reinterpret_cast<char* const>(0x73000018);
static char* const OUT_Y_ADDR0 = reinterpret_cast<char* const>(0x7300001c);
static char* const OUT_Z_ADDR0 = reinterpret_cast<char* const>(0x73000020);

static char* const THETA_ADDR1 = reinterpret_cast<char* const>(0x74000000);
static char* const     R_ADDR1 = reinterpret_cast<char* const>(0x74000004);
static char* const     T_ADDR1 = reinterpret_cast<char* const>(0x74000008);
static char* const  IN_X_ADDR1 = reinterpret_cast<char* const>(0x7400000c);
static char* const  IN_Y_ADDR1 = reinterpret_cast<char* const>(0x74000010);
static char* const  IN_Z_ADDR1 = reinterpret_cast<char* const>(0x74000014);
static char* const OUT_X_ADDR1 = reinterpret_cast<char* const>(0x74000018);
static char* const OUT_Y_ADDR1 = reinterpret_cast<char* const>(0x7400001c);
static char* const OUT_Z_ADDR1 = reinterpret_cast<char* const>(0x74000020);
// DMA 
static volatile uint32_t * const DMA_SRC_ADDR  = (uint32_t * const)0x70000000;
static volatile uint32_t * const DMA_DST_ADDR  = (uint32_t * const)0x70000004;
static volatile uint32_t * const DMA_LEN_ADDR  = (uint32_t * const)0x70000008;
static volatile uint32_t * const DMA_OP_ADDR   = (uint32_t * const)0x7000000C;
static volatile uint32_t * const DMA_STAT_ADDR = (uint32_t * const)0x70000010;
static const uint32_t DMA_OP_MEMCPY = 1;

bool _is_using_dma = true;

void write_data_to_ACC(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){  
    // Using DMA 
    *DMA_SRC_ADDR = (uint32_t)(buffer);
    *DMA_DST_ADDR = (uint32_t)(ADDR);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR  = DMA_OP_MEMCPY;
  }else{
    // Directly Send
    memcpy(ADDR, buffer, sizeof(unsigned char)*len);
  }
}
void read_data_from_ACC(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){
    // Using DMA 
    *DMA_SRC_ADDR = (uint32_t)(ADDR);
    *DMA_DST_ADDR = (uint32_t)(buffer);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR  = DMA_OP_MEMCPY;
  }else{
    // Directly Read
    memcpy(buffer, ADDR, sizeof(unsigned char)*len);
  }
}

void sprintfloat(char* buf, float num) {
	const char* tmpSign = (num < 0) ? "-" : "";
	float tmpVal = (num < 0) ? -num : num;

	int tmpInt1 = (int) tmpVal;                  // Get the integer (678).
	float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
	int tmpInt2 = trunc(tmpFrac * 10000000);  // Turn into integer (123).

	// Print as parts, note that you need 0-padding for fractional bit.
	sprintf(buf, "%s%d.%07d", tmpSign, tmpInt1, tmpInt2);
}