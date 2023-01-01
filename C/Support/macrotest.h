#define CHECK_VAL_NOT_LESS_0(val, err_string) \
    do                                        \
    {                                         \
        if (val < 0)                          \
        {                                     \
            printf("%s\n", err_string);       \
        }                                     \
    } while (0)

#define MAX(a,b, result)  \
    do                                        \
    {                                         \
        if (a>b)                          \
        {                                     \
            *result = a;              \
        }else{*result=b;}                                     \
    } while (0)            
            