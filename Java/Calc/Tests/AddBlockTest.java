package Calc.Tests;
import Calc.ExceptionHierarchy.Math.NotEnoughArgumentsException;
import Calc.General.Context;
import Calc.Math.AddBlock;
import org.junit.Test;

public class AddBlockTest
{
    @Test
    public void simpleTest1()
    {
        AddBlock addBlock = new AddBlock();
        String[] sequenceFromCommandList = {"ADD"};
        Context context = new Context();

        context.getStack().push((double)5);
        context.getStack().push((double)2);

        addBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 7);
    }

    @Test
    public void simpleTest2()
    {
        AddBlock addBlock = new AddBlock();
        String[] sequenceFromCommandList = {"ADD"};
        Context context = new Context();

        context.getStack().push((double)125);
        context.getStack().push((double)125);

        addBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 250);
    }

    @Test
    public void simpleTest3()
    {
        AddBlock addBlock = new AddBlock();
        String[] sequenceFromCommandList = {"ADD"};
        Context context = new Context();

        context.getStack().push((double)15652);
        context.getStack().push((double)-15925);

        addBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == -273);
    }

    @Test
    public void simpleTest4()
    {
        AddBlock addBlock = new AddBlock();
        String[] sequenceFromCommandList = {"ADD"};
        Context context = new Context();

        context.getStack().push((double)-15652);
        context.getStack().push((double)-15925);

        addBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == -31577);
    }

    @Test
    public void simpleTest5()
    {
        AddBlock addBlock = new AddBlock();
        String[] sequenceFromCommandList = {"ADD"};
        Context context = new Context();

        context.getStack().push((double)-15652);
        context.getStack().push((double)15652);

        addBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test
    public void simpleTest6()
    {
        AddBlock addBlock = new AddBlock();
        String[] sequenceFromCommandList = {"ADD"};
        Context context = new Context();

        context.getStack().push((double)0);
        context.getStack().push((double)0);

        addBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test (expected = NotEnoughArgumentsException.class)
    public void badStackTest1()
    {
        AddBlock addBlock = new AddBlock();
        String[] sequenceFromCommandList = {"ADD"};
        Context context = new Context();

        addBlock.process(sequenceFromCommandList, context);
    }

    @Test (expected = NotEnoughArgumentsException.class)
    public void badStackTest2()
    {
        AddBlock addBlock = new AddBlock();
        String[] sequenceFromCommandList = {"ADD"};
        Context context = new Context();

        context.getStack().push((double)1);

        addBlock.process(sequenceFromCommandList, context);

        assert(context.getStack().peek() == 0);
    }

    @Test
    public void bigStackTest()
    {
        AddBlock addBlock = new AddBlock();
        String[] sequenceFromCommandList = new String[1000 - 1];
        Context context = new Context();
        int result = 0;

        for (int i = 0; i != 1000 - 1; ++i)
        {
            sequenceFromCommandList[i] = "ADD";
        }

        for (int i = 1; i != 1001; ++i)
        {
            result += i;
            context.getStack().push((double)i);
        }

        while (context.getStack().size() >= 2)
        {
            addBlock.process(sequenceFromCommandList, context);
        }

        assert(context.getStack().peek() == result);
    }
}