package Calc.StackOperations;

import Calc.General.Context;
import Calc.General.UniversalCommand;

import java.util.EmptyStackException;

public class PopBlock implements UniversalCommand
{
    public void process(String[] sequenceFromCommandList, Context context)
    {
        if (context.getStack().size() > 0)
        {
            context.getStack().pop();
        }
        else
        {
            throw new EmptyStackException();
        }
    }
}
