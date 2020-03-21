package Calc.Math;
import Calc.ExceptionHierarchy.Math.NegativeSqrtException;
import Calc.ExceptionHierarchy.Math.NotEnoughArgumentsException;
import Calc.General.Context;
import Calc.General.Main;
import Calc.General.UniversalCommand;

import java.util.logging.Level;

public class SqrtBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        if (context.getStack().size() < 1)
        {
            String cause = "SQRT: Impossible to execute a command. Command SQRT needs at least 1 variable in stack";
            System.out.println(cause);
            Main.logger.log(Level.WARNING, cause);
            throw new NotEnoughArgumentsException(1);
        }
        else
        {
            double x = context.getStack().pop();

            if (x >= 0)
            {
                context.getStack().push(Math.sqrt(x));
                Main.logger.log(Level.INFO, "SQRT: return Math.sqrt(" + x + ")" + " = " + Math.sqrt(x));
            }
            else
            {
                Main.logger.log(Level.WARNING, "SQRT: Negative number in sqrt");
                throw new NegativeSqrtException("Negative number in sqrt");
            }
        }
    }
}

