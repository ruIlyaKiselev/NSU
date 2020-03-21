package Calc.Tests;
import Calc.ExceptionHierarchy.Math.NotEnoughArgumentsException;
import Calc.General.Context;
import Calc.IO.PrintBlock;
import org.junit.Test;

public class PrintBlockTest
{
    @Test
    public void simpleTest1()
    {
        PrintBlock printBlock = new PrintBlock();
        String[] sequenceFromCommandList = {"PRINT"};
        Context context = new Context();

        context.getStack().push((double)7);

        printBlock.process(sequenceFromCommandList, context); /* watch console - it'll print 7 */
    }

    @Test (expected = NotEnoughArgumentsException.class)
    public void badStackTest1()
    {
        PrintBlock printBlock = new PrintBlock();
        String[] sequenceFromCommandList = {"PRINT"};
        Context context = new Context();

        printBlock.process(sequenceFromCommandList, context);
    }
}