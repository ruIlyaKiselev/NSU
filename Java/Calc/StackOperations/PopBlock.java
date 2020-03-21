package Calc.StackOperations;
import Calc.General.Context;
import Calc.General.Main;
import Calc.General.UniversalCommand;
import java.util.EmptyStackException;
import java.util.logging.Level;

public class PopBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        if (context.getStack().size() > 0)
        {
            context.getStack().pop();
            Main.logger.log(Level.INFO, "POP: pop last value");
        }
        else
        {
            Main.logger.log(Level.WARNING, "POP: Empty stack");
            throw new EmptyStackException();
        }
    }
}
