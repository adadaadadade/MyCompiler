struct t1
{
    int[5][5][5] arr;
    int b;
};

typedef struct t1 st1;


int main()
{
    st1 t;
    int i1;
    int i2;
    int i3;
    int ret;
    ret = 0;
    for(i1 = 0; i1 < 5; i1 ++)
        for(i2 = 0; i2 < 5; i2 ++)
            for(i3 = 0; i3 < 5; i3 ++)
            {
                t.arr[i1][i2][i3] = ret;
                ret ++;
            }
    ret = t.arr[1][1][1];
    return ret;
}