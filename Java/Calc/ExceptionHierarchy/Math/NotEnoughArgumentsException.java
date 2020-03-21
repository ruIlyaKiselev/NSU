package Calc.ExceptionHierarchy.Math;

public class NotEnoughArgumentsException extends IllegalArgumentException
{
    static final String message = "Need more arguments in stack!\n";

    public NotEnoughArgumentsException(int needCount)
    {
        super(message + "For this operation need " + needCount + " arguments");
    }
    public NotEnoughArgumentsException()
    {
        super(message);
    }
}
