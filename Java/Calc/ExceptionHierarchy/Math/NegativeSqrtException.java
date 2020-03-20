package Calc.ExceptionHierarchy.Math;

public class NegativeSqrtException extends ArithmeticException
{
    static final String message = "Negative value in root!\n";

    public NegativeSqrtException(String inputCause, double inputNumber)
    {
        super(message + inputCause + "\nNumber : " + inputNumber);
    }

    public NegativeSqrtException(String inputCause)
    {
        super(message + "Division by zero!\n" + inputCause);
    }

    public NegativeSqrtException(double inputNumber)
    {
        super(message + inputNumber);
    }
}
