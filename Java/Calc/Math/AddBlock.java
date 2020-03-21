package Calc.Math;
import Calc.ExceptionHierarchy.Math.NotEnoughArgumentsException;
import Calc.General.Context;
import Calc.General.Main;
import Calc.General.UniversalCommand;

import java.util.logging.Level;

public class AddBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        if (context.getStack().size() < 2)
        {
            String cause = "ADD: Impossible to execute a command. Command ADD needs at least 2 variable in stack";
            //System.out.println(cause);
            Main.logger.log(Level.WARNING, cause);
            throw new NotEnoughArgumentsException(2);
        }
        else
        {
            double x = context.getStack().pop();
            double y = context.getStack().pop();

            context.getStack().push(x + y);
            Main.logger.log(Level.INFO, "ADD: return " + x + " + " + y + " = " + (x + y));
        }
    }
}
