package Calc.Factory;

import Calc.General.Main;
import Calc.General.UniversalCommand;

import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.io.IOException;
import java.util.logging.Level;

public class BlockFactory
{
    String configurationFileName = "commands.properties";
    private Map<String, UniversalCommand> commandMap = new HashMap<>();

    public void InitFactory()
    {
        Properties prop = new Properties();

        try(InputStream in = BlockFactory.class.getResourceAsStream(configurationFileName))
        {
            prop.load(in);
        }
        catch (IOException e)
        {
            Main.logger.log(Level.WARNING, "Factory: Problem in reading configuration file");
            //System.out.println("Factory: Problem in reading configuration file");
        }

        for (String key : prop.stringPropertyNames())
        {
            String value = prop.get(key).toString();

            try
            {
                Class<?> prototype = Class.forName(value);
                commandMap.put(key, (UniversalCommand) prototype.newInstance());
            }
            catch (Exception e)
            {
                Main.logger.log(Level.WARNING, "Factory: Problem with loading class " + key);
                //System.out.println("Factory: Problem with loading class " + key );
            }
        }
    }

    public UniversalCommand getWorkerForName (String name)
    {
        return commandMap.get(name);
    }
}
