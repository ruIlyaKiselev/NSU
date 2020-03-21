package Calc.Tests;
import Calc.General.Context;
import Calc.StackOperations.PopBlock;
import org.junit.Test;
import java.util.EmptyStackException;

public class PopBlockTest
{
    @Test
    public void simpleTest1()
    {
        PopBlock popBlock = new PopBlock();
        String[] sequenceFromCommandList = {"POP"};
        Context context = new Context();
        context.getStack().push((double)5);
        context.getStack().push((double)7);

        popBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 5);
    }

    @Test (expected = EmptyStackException.class)
    public void badStackTest1()
    {
        PopBlock popBlock = new PopBlock();
        String[] sequenceFromCommandList = {"POP"};
        Context context = new Context();

        popBlock.process(sequenceFromCommandList, context);
    }
}