//read interrupt register

int read_cyp(int adapter, unsigned char address, int len, char buf[])
{
    i2c_setslave(adapter, address);
    return i2c_readbytes(adapter, buf, len);


}
    
//read interrupt register
    union i2c_smbus_data read_data;//data is copied into here;
    struct i2c_smbus_ioctl_data read_args;

    read_args.read_write = I2C_SMBUS_READ;
    read_args.command = 0x29; //interrupt register address 
    read_args.size = I2C_SMBUS_BYTE;
    read_args.data = &read_data;
    int r = ioctl(cypress, I2C_SMBUS, &read_args);  

//    int r = read_cyp(cypress, 0x20, sizeof readbyte, &readbyte);

    printf("ret: %d. read: %d\n ", r, read_data.byte);
