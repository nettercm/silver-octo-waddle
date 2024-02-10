#include <stdio.h>
#include <stdint.h>


/*
This should get flagged as an issue
*/
uint32_t t1(uint8_t *buffer)
{
    uint32_t *p = (uint32_t *)buffer;
    return *p;
}



/*
This should get flagged as an issue
*/
uint32_t t2(uint16_t *buffer)
{
    uint32_t *p = (uint32_t *)buffer;
    return *p;
}



/*
This should get flagged as an issue
*/
uint16_t t3(uint32_t *buffer)
{
    uint16_t *p = (uint16_t *)buffer;
    return *p;
}



/*
The output of this function will depend on endianness of the target platform, but it is not necessarily an issue. It is intentional.
*/
uint8_t t4(void)
{
    volatile int i = 1;
    uint8_t *p = (uint8_t *)&i;

    if (p[0] == 1)
        return 0; //little endian
    else
        return 1; //big endian
}



/*
This one is not necessarily an issue, for example if the buffer comes from a CAN message and the data is in little endian format inside the CAN message.

Of course we write a 32-bit value to this buffer in one part of the code, and then extract bytes and reassemble them in another part of the code, then we have
endianness issues.
*/
uint32_t t5(uint8_t *buffer)
{
    uint32_t u = 0;
    u |= (uint32_t)(buffer[0]) << 0;
    u |= (uint32_t)(buffer[1]) << 8;
    u |= (uint32_t)(buffer[2]) << 16;
    u |= (uint32_t)(buffer[3]) << 24;
    return u;
}



/*
This one is not necessarily an issue, for example if the buffer comes from a CAN message and the data is in BIG endian format inside the CAN message.

Of course we write a 32-bit value to this buffer in one part of the code, and then extract bytes and reassemble them in another part of the code, then we have
endianness issues.
*/
uint32_t t6(uint8_t *buffer)
{
    uint32_t u = 0;
    u |= (uint32_t)(buffer[0]) << 24;
    u |= (uint32_t)(buffer[1]) << 16;
    u |= (uint32_t)(buffer[2]) << 8;
    u |= (uint32_t)(buffer[3]) << 0;
    return u;
}



/*
A copy of t5(), but we will use it differently in other parts of the code.
*/
uint32_t t7(uint8_t *buffer)
{
    uint32_t u = 0;
    u |= (uint32_t)(buffer[0]) << 0;
    u |= (uint32_t)(buffer[1]) << 8;
    u |= (uint32_t)(buffer[2]) << 16;
    u |= (uint32_t)(buffer[3]) << 24;
    return u;
}



uint8_t buffer1[4] = {0x11, 0x22, 0x33, 0x44};

uint8_t buffer2[4];

int main()
{
    printf("Hello, World!\n");

    printf("t1: %x\n", t1((uint8_t  *)buffer1));
    printf("t2: %x\n", t2((uint16_t *)buffer1));
    printf("t3: %x\n", t3((uint32_t *)buffer1));
    printf("t4: %x\n", t4());
    printf("t5: %x\n", t5(buffer1));
    printf("t6: %x\n", t6(buffer1));

    *(uint32_t *)buffer2 = 0x11223344;
    printf("t7: %x\n", t6(buffer2));

    return 0;
}
