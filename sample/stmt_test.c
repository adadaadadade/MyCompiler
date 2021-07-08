int main()
{
    int a;
    if(1)
    {
        a = 1;
    }
    else
    {
        a = 2;
    }
    a = 0;

    while (a < 5)
    {
        a ++;
        if(a == 3)
            break;
    }

    for(a = 0; a < 5; a ++)
    {
        continue;
        a = 245;
    }
    return a;
    
}