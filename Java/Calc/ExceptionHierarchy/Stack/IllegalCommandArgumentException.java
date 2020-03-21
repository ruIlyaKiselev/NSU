package Calc.ExceptionHierarchy.Stack;

public class IllegalCommandArgumentException extends NumberFormatException
{
    static final String message = "Invalid argument value in command. Double value expected.";

    public IllegalCommandArgumentException ()
    {
        super(message);
    }

    public IllegalCommandArgumentException (String additionalMessage)
    {
        super(message + " " + additionalMessage);
    }
}
