int sum(int a, int b, int c, int d, int e)
{
    return a + b + c + d + e;
}

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
   int[5] arr;
   int x;
   arr[1] = 1;
   x = sum(fib(arr[1]), 2, 3, 4, 5);
   x = fib(sum(1, 1, 1, 1, 1));
   return x;
}