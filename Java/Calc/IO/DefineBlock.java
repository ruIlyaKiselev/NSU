package Calc.IO;
import Calc.General.Context;
import Calc.General.Main;
import Calc.General.UniversalCommand;

import java.util.logging.Level;

public class DefineBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        try
        {
            context.getVariables().put(sequenceFromCommandList[1], Double.valueOf(sequenceFromCommandList[2]));
            Main.logger.log(Level.INFO, "DEFINE: store " + sequenceFromCommandList[1] + " as " +
                    sequenceFromCommandList[2]);
        }
        catch (ArrayIndexOutOfBoundsException e)
        {
            String cause = "DEFINE: Bad index of array!";
            //System.out.println(cause);
            Main.logger.log(Level.WARNING, cause);
            throw new ArrayIndexOutOfBoundsException(cause);
        }
        catch (NumberFormatException e)
        {
            String cause = "DEFINE: Incorrect data in arguments. First argument should be a String, " +
                    "second should be a double";
            //System.out.println(cause);
            Main.logger.log(Level.WARNING, cause);
            throw new NumberFormatException(cause);
        }
    }
}
