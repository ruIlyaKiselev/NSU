package Calc.Math;

import Calc.ExceptionHierarchy.Math.DivisionByZeroException;
import Calc.ExceptionHierarchy.Math.NotEnoughArgumentsException;
import Calc.General.Context;
import Calc.General.Main;
import Calc.General.UniversalCommand;

import java.util.logging.Level;

public class DivBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        if (context.getStack().size() < 2)
        {
            String cause = "DIV: Impossible to execute a command. Command DIV needs at least 2 variable in stack";
            //System.out.println(cause);
            Main.logger.log(Level.WARNING, cause);
            throw new NotEnoughArgumentsException(2);
        }
        else
        {
            double x = context.getStack().pop();
            double y = context.getStack().pop();

            if (y != 0)
            {
                context.getStack().push(x / y);
                Main.logger.log(Level.INFO, "DIV: return " + x + " / " + y + " = " + (x / y));
            }
            else
            {
                Main.logger.log(Level.WARNING, "DIV: Division by zero");
                throw new DivisionByZeroException("Division by zero");
            }
        }
    }
}

