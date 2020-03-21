package Calc.Tests;
import Calc.General.Context;
import Calc.IO.DefineBlock;
import org.junit.Test;

public class DefineBlockTest
{
    @Test
    public void simpleTest1()
    {
        DefineBlock defineBlock = new DefineBlock();
        String[] sequenceFromCommandList = {"DEFINE", "seven",  "7"};
        Context context = new Context();

        defineBlock.process(sequenceFromCommandList, context);

        assert(context.getVariables().get("seven") == 7);
    }

    @Test (expected = ArrayIndexOutOfBoundsException.class)
    public void badVarTest1()
    {
        DefineBlock defineBlock = new DefineBlock();
        String[] sequenceFromCommandList = {"DEFINE", "seven"};
        Context context = new Context();

        defineBlock.process(sequenceFromCommandList, context);
    }

    @Test (expected = NumberFormatException.class)
    public void badVarTest2()
    {
        DefineBlock defineBlock = new DefineBlock();
        String[] sequenceFromCommandList = {"DEFINE", "seven", "hello there"};
        Context context = new Context();

        defineBlock.process(sequenceFromCommandList, context);
    }
}