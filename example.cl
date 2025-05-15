func main 
{
    x:10
    y:20
    
    if (x > y)
    {
        x:11
    }

    else
    {
        y:21
    }

    z:(x+y*2)
    log(2, z)

    till(z>50)
    {
        z:(z - 5)
        log(2, z)
    }

    result: (sin(z)^2 + cos(z)^2)
}