int main()
{
    int a;
    if (1)
    {
        a = 1;
        assert(-1, 1);
    }
    else
    {
        a = 2;
    }
    a = 0;

    while (a < 5)
    {
        a++;
        if (a == 3)
            break;
        while (a)
        {
            if (a != 5)
                break;
        }
    }
    return a;
    for (a = 0; a < 5; a++)
    {
        continue;
        a = 245;
    }
    return a;
}