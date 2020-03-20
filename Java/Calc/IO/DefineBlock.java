package Calc.IO;

import Calc.General.Context;
import Calc.General.UniversalCommand;

public class DefineBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        try
        {
            context.getVariables().put(sequenceFromCommandList[1], Double.valueOf(sequenceFromCommandList[2]));
        }
        catch (ArrayIndexOutOfBoundsException e)
        {
            System.out.println("Define: Bad index of array!");
        }
        catch (ArrayStoreException e)
        {
            System.out.println("Define: Incorrect data in arguments. First argument should be a String, " +
                    "second should be a double");
        }
    }
}
