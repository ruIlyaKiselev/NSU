package Calc.Tests;
import Calc.ExceptionHierarchy.Math.NegativeSqrtException;
import Calc.ExceptionHierarchy.Math.NotEnoughArgumentsException;
import Calc.General.Context;
import Calc.Math.SqrtBlock;
import org.junit.Test;

public class SqrtBlockTest
{
    @Test
    public void simpleTest1()
    {
        SqrtBlock sqrtBlock = new SqrtBlock();
        String[] sequenceFromCommandList = {"SQRT"};
        Context context = new Context();

        context.getStack().push((double)4);

        sqrtBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 2);
    }

    @Test
    public void simpleTest2()
    {
        SqrtBlock sqrtBlock = new SqrtBlock();
        String[] sequenceFromCommandList = {"SQRT"};
        Context context = new Context();

        context.getStack().push((double)125);

        sqrtBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == Math.sqrt(125));
    }

    @Test
    public void simpleTest3()
    {
        SqrtBlock sqrtBlock = new SqrtBlock();
        String[] sequenceFromCommandList = {"SQRT"};
        Context context = new Context();

        context.getStack().push((double)15652);

        sqrtBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == Math.sqrt(15652));
    }

    @Test (expected = NegativeSqrtException.class)
    public void simpleTest4()
    {
        SqrtBlock sqrtBlock = new SqrtBlock();
        String[] sequenceFromCommandList = {"SQRT"};
        Context context = new Context();

        context.getStack().push((double)-15925);

        sqrtBlock.process(sequenceFromCommandList, context);
    }

    @Test
    public void simpleTest5()
    {
        SqrtBlock sqrtBlock = new SqrtBlock();
        String[] sequenceFromCommandList = {"SQRT"};
        Context context = new Context();

        context.getStack().push((double)0);

        sqrtBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test (expected = NotEnoughArgumentsException.class)
    public void badStackTest1()
    {
        SqrtBlock sqrtBlock = new SqrtBlock();
        String[] sequenceFromCommandList = {"SQRT"};
        Context context = new Context();

        sqrtBlock.process(sequenceFromCommandList, context);
    }
}