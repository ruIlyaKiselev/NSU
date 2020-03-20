package Calc.General;

import Calc.Factory.BlockFactory;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Main
{
    public static void main(String[] args) throws FileNotFoundException
    {
        Scanner scanner = null;
        Context context = new Context();

        if (args.length > 0)
        {
            scanner = new Scanner(new FileInputStream(args[0]));
        }

        BlockFactory workerFactory = new BlockFactory();
        workerFactory.InitFactory();

        String tmp;

        assert scanner != null;
        while (scanner.hasNextLine())
        {
            tmp = scanner.nextLine();
            String[] str = tmp.split(" ");

            UniversalCommand executor = workerFactory.getWorkerForName(str[0]);

            if (executor != null)
            {
                try
                {
                    executor.process(str, context);
                }
                catch (Exception e)
                {
                    System.out.println("Unknown command " + e.getMessage());
                    throw new UnsupportedOperationException();
                }
            }
            else
            {
                System.out.println("There is no commands");
                throw new NullPointerException();
            }
        }
    }
}