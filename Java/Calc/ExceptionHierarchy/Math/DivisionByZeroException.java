package Calc.ExceptionHierarchy.Math;

public class DivisionByZeroException extends ArithmeticExceptionWithCause
{
    static final String message = "Division by zero!\n";

    public DivisionByZeroException(String inputCause, double inputNumber)
    {
        super(message + inputCause + "\nNumber : " + inputNumber);
    }

    public DivisionByZeroException(String inputCause)
    {
        super(message + "Division by zero!\n" + inputCause);
    }

    public DivisionByZeroException(double inputNumber)
    {
        super(message + inputNumber);
    }
}

