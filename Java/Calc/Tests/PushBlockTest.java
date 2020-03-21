package Calc.Tests;
import Calc.ExceptionHierarchy.Stack.IllegalCommandArgumentException;
import Calc.General.Context;
import Calc.StackOperations.PushBlock;
import org.junit.Test;

public class PushBlockTest
{
    @Test
    public void simpleTest1()
    {
        PushBlock pushBlock = new PushBlock();
        String[] sequenceFromCommandList = {"PUSH", "5"};
        Context context = new Context();

        pushBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 5);
    }

    @Test
    public void simpleTest2()
    {
        PushBlock pushBlock = new PushBlock();
        String[] sequenceFromCommandList = {"PUSH", "-5"};
        Context context = new Context();

        pushBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == -5);
    }

    @Test
    public void simpleTest3()
    {
        PushBlock pushBlock = new PushBlock();
        String[] sequenceFromCommandList = {"PUSH", "0"};
        Context context = new Context();

        pushBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test (expected = IllegalCommandArgumentException.class)
    public void badStackTest1()
    {
        PushBlock pushBlock = new PushBlock();
        String[] sequenceFromCommandList = {"PUSH", "five"};
        Context context = new Context();

        pushBlock.process(sequenceFromCommandList, context);
    }
}