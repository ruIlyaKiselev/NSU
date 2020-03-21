package Calc.IO;
import Calc.ExceptionHierarchy.Math.NotEnoughArgumentsException;
import Calc.General.Context;
import Calc.General.Main;
import Calc.General.UniversalCommand;

import java.util.logging.Level;

public class PrintBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        if (context.getStack().size() == 0)
        {
            String cause = "Print: Impossible to execute a command. Command PRINT needs at least 1 variable in stack";
            //System.out.println(cause);
            Main.logger.log(Level.WARNING, cause);
            throw new NotEnoughArgumentsException(1);
        }
        else
        {
            System.out.println(context.getStack().peek());
            Main.logger.log(Level.INFO, "PRINT: print " + context.getStack().peek());
        }
    }
}
