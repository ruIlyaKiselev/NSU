package Calc.Math;

import Calc.General.Context;
import Calc.General.UniversalCommand;

public class MulBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        if (context.getStack().size() < 2)
        {
            System.out.println("Mul: Impossible to execute a command. Command MUL needs at least 2 variable in stack");
        }
        else
        {
            double x = context.getStack().pop();
            double y = context.getStack().pop();

            context.getStack().push(x * y);
        }
    }
}
