package Calc.General;
import Calc.Factory.BlockFactory;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.HashSet;
import java.util.Scanner;
import java.util.Set;
import java.util.logging.*;

public class Main
{
    public static final Logger logger = Logger.getLogger("Log");

    public static void main(String[] args)
    {

        FileHandler fh;

        try
        {
            fh = new FileHandler("MyLogFile.log");
            logger.addHandler(fh);
            SimpleFormatter formatter = new SimpleFormatter();
            fh.setFormatter(formatter);
        }
        catch (IOException e)
        {
            System.out.println("Problem in logs file");
        }

        Scanner scanner = null;
        Context context = new Context();

        if (args.length == 0)
        {
            scanner = new Scanner(System.in);
            logger.log(Level.WARNING, "Reading commands from console.");
        }
        else
        {
            try
            {
                scanner = new Scanner(new FileInputStream(args[0]));
                logger.log(Level.INFO, "Reading commands from " + args[0] + " file.");
            }
            catch (FileNotFoundException e)
            {
                System.out.println("Problem in reading command list");
            }
        }

        BlockFactory workerFactory = new BlockFactory();
        workerFactory.InitFactory();

        String tmp;
        Set<String> commandsForCheck = new HashSet<String>();
        commandsForCheck.add("DEFINE");
        commandsForCheck.add("PRINT");
        commandsForCheck.add("ADD");
        commandsForCheck.add("DIV");
        commandsForCheck.add("MUL");
        commandsForCheck.add("SQRT");
        commandsForCheck.add("SUB");
        commandsForCheck.add("POP");
        commandsForCheck.add("PUSH");

        assert scanner != null;
        while (scanner.hasNextLine())
        {
            tmp = scanner.nextLine();
            String[] str = tmp.split(" ");

            if (commandsForCheck.contains(str[0]))
            {
                UniversalCommand executor = workerFactory.getWorkerForName(str[0]);

                if (executor != null)
                {
                    try
                    {
                        executor.process(str, context);
                    }
                    catch (Exception e)
                    {
                        logger.log(Level.WARNING, "Unknown command for executor! " + e.getMessage().toString());
                        //System.out.println("Unknown command: " + e.getMessage());
                        //throw new UnsupportedOperationException();
                    }
                }
                else
                {
                    logger.log(Level.WARNING, "There is no commands");
                    //System.out.println("There is no commands");
                    throw new NullPointerException();
                }
            }
        }
    }
}