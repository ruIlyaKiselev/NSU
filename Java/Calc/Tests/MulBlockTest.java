package Calc.Tests;
import Calc.ExceptionHierarchy.Math.NotEnoughArgumentsException;
import Calc.General.Context;
import Calc.Math.MulBlock;
import org.junit.Test;

public class MulBlockTest
{
    @Test
    public void simpleTest1()
    {
        MulBlock mulBlock = new MulBlock();
        String[] sequenceFromCommandList = {"MUL"};
        Context context = new Context();

        context.getStack().push((double)5);
        context.getStack().push((double)2);

        mulBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 10);
    }

    @Test
    public void simpleTest2()
    {
        MulBlock mulBlock = new MulBlock();
        String[] sequenceFromCommandList = {"MUL"};
        Context context = new Context();

        context.getStack().push((double)125);
        context.getStack().push((double)125);

        mulBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 125*125);
    }

    @Test
    public void simpleTest3()
    {
        MulBlock mulBlock = new MulBlock();
        String[] sequenceFromCommandList = {"MUL"};
        Context context = new Context();

        context.getStack().push((double)15652);
        context.getStack().push((double)-15925);

        mulBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == -15652 * 15925);
    }

    @Test
    public void simpleTest4()
    {
        MulBlock mulBlock = new MulBlock();
        String[] sequenceFromCommandList = {"MUL"};
        Context context = new Context();

        context.getStack().push((double)-15652);
        context.getStack().push((double)-15925);

        mulBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 15652 * 15925);
    }

    @Test
    public void simpleTest5()
    {
        MulBlock mulBlock = new MulBlock();
        String[] sequenceFromCommandList = {"MUL"};
        Context context = new Context();

        context.getStack().push((double)-15652);
        context.getStack().push((double)0);

        mulBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test
    public void simpleTest6()
    {
        MulBlock mulBlock = new MulBlock();
        String[] sequenceFromCommandList = {"MUL"};
        Context context = new Context();

        context.getStack().push((double)0);
        context.getStack().push((double)0);

        mulBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test (expected = NotEnoughArgumentsException.class)
    public void badStackTest1()
    {
        MulBlock mulBlock = new MulBlock();
        String[] sequenceFromCommandList = {"MUL"};
        Context context = new Context();

        mulBlock.process(sequenceFromCommandList, context);
    }

    @Test (expected = NotEnoughArgumentsException.class)
    public void badStackTest2()
    {
        MulBlock mulBlock = new MulBlock();
        String[] sequenceFromCommandList = {"MUL"};
        Context context = new Context();

        context.getStack().push((double)1);

        mulBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test
    public void bigStackTest()
    {
        MulBlock mulBlock = new MulBlock();
        String[] sequenceFromCommandList = new String[10 - 1];
        Context context = new Context();
        int result = 1;

        for (int i = 0; i != 10 - 1; ++i)
        {
            sequenceFromCommandList[i] = "MUL";
        }

        for (int i = 2; i != 10 + 1; ++i)
        {
            result *= i;
            context.getStack().push((double)i);
        }

        while (context.getStack().size() >= 2)
        {
            mulBlock.process(sequenceFromCommandList, context);
        }

        assert(context.getStack().peek() == result);
    }
}