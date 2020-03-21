package Calc.StackOperations;
import Calc.ExceptionHierarchy.Stack.IllegalCommandArgumentException;
import Calc.General.Context;
import Calc.General.Main;
import Calc.General.UniversalCommand;

import java.util.logging.Level;

public class PushBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        if (context.getVariables().containsKey(sequenceFromCommandList[1]))
        {
            try
            {
                context.getStack().push(context.getVariables().get(sequenceFromCommandList[1]));
                Main.logger.log(Level.INFO, "PUSH: PUSH " + sequenceFromCommandList[1]);
            }
            catch (NumberFormatException e)
            {
                Main.logger.log(Level.WARNING, "PUSH: Bad value");
                throw new IllegalCommandArgumentException();
            }
        }
        else
        {
            try
            {
                context.getStack().push(Double.valueOf(sequenceFromCommandList[1]));
                Main.logger.log(Level.INFO, "PUSH: PUSH " + sequenceFromCommandList[1]);
            }
            catch (NumberFormatException e)
            {
                Main.logger.log(Level.WARNING, "PUSH: Bad value");
                throw new IllegalCommandArgumentException();
            }
        }
    }
}
