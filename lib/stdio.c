#include "stdio.h"
#include "HalUart.h"
#include "stdint.h"

static char printf_buf[PRINTF_BUF_LEN];


uint32_t putstr(const char* s){
    uint32_t c = 0;
    while (*s)
    {
        Hal_uart_put_char(*s++);
        c++;
    }
    return c;
}

void putchar(const char c){
    Hal_uart_put_char(c);
}

uint8_t getchar(){
    return Hal_uart_get_char();
}

uint32_t getstr(char* s, uint32_t len){
    uint32_t c = 0;
    uint8_t tmp;
    while(c < len){
        tmp = Hal_uart_get_char();

        //echo
        Hal_uart_put_char(tmp);

        if(!tmp || !(tmp - 0x0d)){
            *(s + c) = 0x00;
            return c;
        }
        *(s + c) = tmp;
        c++;
    }
    return c;
}

uint32_t debug_printf(const char* format, ...){
    va_list args;
    va_start(args, format);
    vsprintf(printf_buf, format, args);
    va_end(args);

    return putstr(printf_buf);
}

uint32_t vsprintf(char* buf, const char* format, va_list arg){
    uint32_t c_buf = 0;
    char ch_tmp;
    char* str_tmp;
    uint32_t u_int_tmp;
    uint32_t hex_tmp;


    for(uint32_t i = 0; format[i] && (c_buf < PRINTF_BUF_LEN); i++){
        
        //format string
        if(format[i] == '%'){
            //for '%'
            i++;

            switch(format[i]){
                case 'c':
                    ch_tmp = (char)va_arg(arg, uint32_t);
                    buf[c_buf++] = ch_tmp;
                    break;

                case 's':
                    str_tmp = (char*)va_arg(arg, uint32_t);
                    if(!str_tmp){
                        putstr("[Error] segmentation fault: Null pointer reference while processing formate string\n");
                        buf[0] = '\0';
                        return 0;
                    }
                    while(*str_tmp && (c_buf < PRINTF_BUF_LEN)){
                        buf[c_buf++] = (*str_tmp++);
                    }
                    break;

                case 'x':
                    hex_tmp = (uint32_t)va_arg(arg, uint32_t);
                    c_buf += utoa(&buf[c_buf], hex_tmp, utoa_hex);
                    break;

                case 'u':
                    u_int_tmp = (uint32_t)va_arg(arg, uint32_t);
                    c_buf += utoa(&buf[c_buf], u_int_tmp, utoa_dec);
                    break;
            }
        }
        else{
            buf[c_buf++] = format[i];
        }
        
    }

    //BOF
    if(c_buf >= PRINTF_BUF_LEN){
        buf[0] = '\0';
        return 0;
    }
    buf[c_buf] = '\0';
    return c_buf;
}

uint32_t utoa(char* buf, uint32_t val, utoa_t base){
    uint32_t c = 0;
    char tmp[11];
    int32_t tmp_i = 0;

    do{
        uint32_t r = val % (uint32_t)base;
        //if r >= 10 in the case which base ==10, r < 10 
        if(r >= 10){
            r += 'A' - '0' - 10;
        }
        tmp[tmp_i] = r + '0';
        val /= base;
        tmp_i++;
    }while(val);

    //reverse
    tmp_i--;
    while(tmp_i >= 0){
        buf[c++] = tmp[tmp_i];
        tmp_i--;
    }

    return c;
}