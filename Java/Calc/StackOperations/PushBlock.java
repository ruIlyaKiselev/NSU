package Calc.StackOperations;

import Calc.General.Context;
import Calc.General.UniversalCommand;

public class PushBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        if (context.getVariables().containsKey(sequenceFromCommandList[1]))
        {
            context.getStack().push(context.getVariables().get(sequenceFromCommandList[1]));
        }
        else
        {
            context.getStack().push(Double.valueOf(sequenceFromCommandList[1]));
        }
    }
}
