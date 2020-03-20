package Calc.Math;

import Calc.ExceptionHierarchy.Math.DivisionByZeroException;
import Calc.General.Context;
import Calc.General.UniversalCommand;

public class DivBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        if (context.getStack().size() < 2)
        {
            System.out.println("Div: Impossible to execute a command. Command DIV needs at least 2 variable in stack");
        }
        else
        {
            double x = context.getStack().pop();
            double y = context.getStack().pop();

            if (y != 0)
            {
                context.getStack().push(x / y);
            }
            else
            {
                throw new DivisionByZeroException("Division by zero");
            }
        }
    }
}

