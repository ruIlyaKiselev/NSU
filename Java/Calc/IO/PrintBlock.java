package Calc.IO;

import Calc.General.Context;
import Calc.General.UniversalCommand;

public class PrintBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        if (context.getStack().size() == 0)
        {
            System.out.println("Print: Impossible to execute a command. Command PRINT needs at least 1 variable in stack");
        }
        else
        {
            System.out.println(context.getStack().peek());
        }
    }
}
