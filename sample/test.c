struct test_struct
{
   int[10] a;
   int b;
   int c;
};


int fib(int n)
{
   if (n <= 2)
   {
      return 1;
   }
   else
   {
      return fib(n - 1) + fib(n - 2);
   }
}
int main()
{
   int[5][5][5] arr;
   struct test_struct t;
   int i1;
   int i2;
   int i3;
   int ret;
   ret = 0;
   t.a[2] = 3;
   for(i1 = 0; i1 < 5; i1 ++)
      for(i2 = 0; i2 < 5; i2 ++)
         for(i3 = 0; i3 < 5; i3 ++)
         {
            ret ++;
            arr[i1][i2][i3] = ret;
         }
            
   ret = arr[4][3][1];
   ret = t.a[2];
   return ret;
}
