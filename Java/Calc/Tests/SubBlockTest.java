package Calc.Tests;
import Calc.ExceptionHierarchy.Math.NotEnoughArgumentsException;
import Calc.General.Context;
import Calc.Math.SubBlock;
import org.junit.Test;

public class SubBlockTest
{
    @Test
    public void simpleTest1()
    {
        SubBlock subBlock = new SubBlock();
        String[] sequenceFromCommandList = {"SUB"};
        Context context = new Context();

        context.getStack().push((double)5);
        context.getStack().push((double)2);

        subBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == -3);
    }

    @Test
    public void simpleTest2()
    {
        SubBlock subBlock = new SubBlock();
        String[] sequenceFromCommandList = {"SUB"};
        Context context = new Context();

        context.getStack().push((double)125);
        context.getStack().push((double)125);

        subBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test
    public void simpleTest3()
    {
        SubBlock subBlock = new SubBlock();
        String[] sequenceFromCommandList = {"SUB"};
        Context context = new Context();

        context.getStack().push((double)15652);
        context.getStack().push((double)-15925);

        subBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == -31577);
    }

    @Test
    public void simpleTest4()
    {
        SubBlock subBlock = new SubBlock();
        String[] sequenceFromCommandList = {"SUB"};
        Context context = new Context();

        context.getStack().push((double)-15652);
        context.getStack().push((double)-15925);

        subBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == -273);
    }

    @Test
    public void simpleTest5()
    {
        SubBlock subBlock = new SubBlock();
        String[] sequenceFromCommandList = {"SUB"};
        Context context = new Context();

        context.getStack().push((double)-15652);
        context.getStack().push((double)15652);

        subBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 31304);
    }

    @Test
    public void simpleTest6()
    {
        SubBlock subBlock = new SubBlock();
        String[] sequenceFromCommandList = {"SUB"};
        Context context = new Context();

        context.getStack().push((double)0);
        context.getStack().push((double)0);

        subBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test (expected = NotEnoughArgumentsException.class)
    public void badStackTest1()
    {
        SubBlock subBlock = new SubBlock();
        String[] sequenceFromCommandList = {"SUB"};
        Context context = new Context();

        subBlock.process(sequenceFromCommandList, context);
    }

    @Test (expected = NotEnoughArgumentsException.class)
    public void badStackTest2()
    {
        SubBlock subBlock = new SubBlock();
        String[] sequenceFromCommandList = {"SUB"};
        Context context = new Context();

        context.getStack().push((double)1);

        subBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test
    public void bigStackTest()
    {
        SubBlock subBlock = new SubBlock();
        String[] sequenceFromCommandList = new String[1000 - 1];
        Context context = new Context();
        int result = 0;

        for (int i = 0; i != 1000 - 1; ++i)
        {
            sequenceFromCommandList[i] = "SUB";
        }

        for (int i = 1; i != 1000; ++i)
        {
            result += i;
            context.getStack().push((double)(-i));
        }
        context.getStack().push((double)(0));

        while (context.getStack().size() >= 2)
        {
            subBlock.process(sequenceFromCommandList, context);
        }

        assert(context.getStack().peek() == result);
    }
}