#ifndef BASETYPE_H
#define BASETYPE_H

#define ARG_DATA_TYPE_NONE 0x00
#define ARG_DATA_TYPE_CHAR 0x01
#define ARG_DATA_TYPE_CHAR_NAME "char"
#define ARG_DATA_TYPE_UCHAR 0x02
#define ARG_DATA_TYPE_UCHAR_NAME "uchar"
#define ARG_DATA_TYPE_SHORT 0x03
#define ARG_DATA_TYPE_SHORT_NAME "short"
#define ARG_DATA_TYPE_USHORT 0x04
#define ARG_DATA_TYPE_USHORT_NAME "ushort"
#define ARG_DATA_TYPE_INT 0x05
#define ARG_DATA_TYPE_INT_NAME "int"
#define ARG_DATA_TYPE_UINT 0x06
#define ARG_DATA_TYPE_UINT_NAME "uint"
#define ARG_DATA_TYPE_LONG 0x07
#define ARG_DATA_TYPE_LONG_NAME "long"
#define ARG_DATA_TYPE_ULONG 0x08
#define ARG_DATA_TYPE_ULONG_NAME "ulong"
#define ARG_DATA_TYPE_FLOAT 0x09
#define ARG_DATA_TYPE_FLOAT_NAME "float"
#define ARG_DATA_TYPE_DOUBLE 0x0A
#define ARG_DATA_TYPE_DOUBLE_NAME "double"
#define ARG_DATA_TYPE_MEM 0x0B
#define ARG_DATA_TYPE_MEM_NAME "cl_mem"

#define ARG_READ_WRITE_NONE 0x00
#define ARG_READ_ONLY 0x01
#define ARG_READ_ONLY_NAME "read_only"
#define ARG_WRITE_ONLY 0x02
#define ARG_WRITE_ONLY_NAME "write_only"
#define ARG_READ_WRITE 0x03
#define ARG_READ_WRITE_NAME "read_write"

#define ARG_DATA_INPUT_OUTPUT_NONE 0x00
#define ARG_DATA_INPUT 0x01
#define ARG_DATA_INPUT_NAME "input"
#define ARG_DATA_OUTPUT 0x02
#define ARG_DATA_OUTPUT_NAME "output"

#define ARG_BOOL_TYPE_TRUE true
#define ARG_BOOL_TYPE_TRUE_NAME  "true"
#define ARG_BOOL_TYPE_FALSE false
#define ARG_BOOL_TYPE_FALSE_NAME "false"

#endif // BASETYPE_H
