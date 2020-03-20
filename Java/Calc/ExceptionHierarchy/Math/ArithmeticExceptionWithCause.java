package Calc.ExceptionHierarchy.Math;

public class ArithmeticExceptionWithCause extends  ArithmeticException
{
    public ArithmeticExceptionWithCause (String inputCause, double inputNumber)
    {
        super(inputCause + "\nNumber : " + inputNumber);
    }

    public ArithmeticExceptionWithCause (String inputCause)
    {
        super(inputCause);
    }

    public ArithmeticExceptionWithCause (double inputNumber)
    {
        super("Number : " + inputNumber);
    }
}
