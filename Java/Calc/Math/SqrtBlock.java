package Calc.Math;

import Calc.ExceptionHierarchy.Math.NegativeSqrtException;
import Calc.General.Context;
import Calc.General.UniversalCommand;

public class SqrtBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        if (context.getStack().size() < 1)
        {
            System.out.println("Sqrt: Impossible to execute a command. Command SQRT needs at least 1 variable in stack");
        }
        else
        {
            double x = context.getStack().pop();

            if (x != 0)
            {
                context.getStack().push(Math.sqrt(x));
            }
            else
            {
                throw new NegativeSqrtException("Negative number in sqrt");
            }
        }
    }
}

