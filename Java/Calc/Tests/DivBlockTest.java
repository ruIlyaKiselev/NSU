package Calc.Tests;
import Calc.ExceptionHierarchy.Math.DivisionByZeroException;
import Calc.ExceptionHierarchy.Math.NotEnoughArgumentsException;
import Calc.General.Context;
import Calc.Math.DivBlock;
import org.junit.Test;

public class DivBlockTest
{
    @Test
    public void simpleTest1()
    {
        DivBlock divBlock = new DivBlock();
        String[] sequenceFromCommandList = {"DIV"};
        Context context = new Context();

        context.getStack().push((double)5);
        context.getStack().push((double)2);

        divBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == (double)(0.4));
    }

    @Test
    public void simpleTest2()
    {
        DivBlock divBlock = new DivBlock();
        String[] sequenceFromCommandList = {"DIV"};
        Context context = new Context();

        context.getStack().push((double)125);
        context.getStack().push((double)125);

        divBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 1);
    }

    @Test
    public void simpleTest3()
    {
        DivBlock divBlock = new DivBlock();
        String[] sequenceFromCommandList = {"DIV"};
        Context context = new Context();

        context.getStack().push((double)15652);
        context.getStack().push((double)-15925);

        divBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == -1.0174418604651163);
    }

    @Test
    public void simpleTest4()
    {
        DivBlock divBlock = new DivBlock();
        String[] sequenceFromCommandList = {"DIV"};
        Context context = new Context();

        context.getStack().push((double)-15652);
        context.getStack().push((double)-15925);

        divBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 1.0174418604651163);
    }

    @Test
    public void simpleTest5()
    {
        DivBlock divBlock = new DivBlock();
        String[] sequenceFromCommandList = {"DIV"};
        Context context = new Context();

        context.getStack().push((double)-15652);
        context.getStack().push((double)0);

        divBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test (expected = DivisionByZeroException.class)
    public void simpleTest6()
    {
        DivBlock divBlock = new DivBlock();
        String[] sequenceFromCommandList = {"DIV"};
        Context context = new Context();

        context.getStack().push((double)0);
        context.getStack().push((double)0);

        divBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test (expected = NotEnoughArgumentsException.class)
    public void badStackTest1()
    {
        DivBlock divBlock = new DivBlock();
        String[] sequenceFromCommandList = {"DIV"};
        Context context = new Context();

        divBlock.process(sequenceFromCommandList, context);
    }

    @Test (expected = NotEnoughArgumentsException.class)
    public void badStackTest2()
    {
        DivBlock divBlock = new DivBlock();
        String[] sequenceFromCommandList = {"DIV"};
        Context context = new Context();

        context.getStack().push((double)1);

        divBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test
    public void bigStackTest()
    {
        DivBlock divBlock = new DivBlock();
        String[] sequenceFromCommandList = new String[10 - 1];
        Context context = new Context();

        for (int i = 0; i != 10 - 1; ++i)
        {
            sequenceFromCommandList[i] = "DIV";
        }

        for (int i = 2; i != 10 + 1; ++i)
        {
            context.getStack().push((double)i);
        }

        while (context.getStack().size() >= 2)
        {
            divBlock.process(sequenceFromCommandList, context);
        }

        System.out.println(context.getStack().peek());
        assert(context.getStack().peek() == 2.7557319223985893E-5);
    }
}