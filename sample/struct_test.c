int func(int a[1][2][3])
{
    //int ***a = arg;
    int i1 = 0;
    int i2 = 1;
    int i3 = 2;
    a[i1][i2][i3] = 1;
    return a[i1][i2][i3];
}

int main()
{
    int a[5][5][5];
    int ;
    sizeof(**a);
    sizeof(a[1]);
    //6, 3, 1
    int i1 = 0;
    int i2 = 1;
    int i3 = 2;
    int ret;
    //24, 12, 4
    a[i1][i2][i3] = 2;
    ret = func(a);
}
