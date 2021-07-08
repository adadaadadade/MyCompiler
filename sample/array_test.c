int ret_a(int a)
{
    return a;
}

int main()
{
    int[5] arr;
    int[5][5] arr2;
    int[5][5][5] arr3;
    int x;

    arr[3] = 5;
    x = arr[3];
    x = x + 5 * arr[3];
    
    x = 1;
    x = arr[x*2 + 1];

    arr2[3][1] = 5;
    x = arr2[3][1];
    x = x + 5 * arr2[3][1];

    arr[1] = 1;
    arr3[3][1][ret_a(1)] = 5;
    x = arr3[3][1][arr[1]];
    x = x + 5 * arr3[3][1][1];
    return x;

}