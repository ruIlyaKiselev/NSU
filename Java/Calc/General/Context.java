package Calc.General;

import java.util.*;

public class Context
{
    Stack<Double> calcStack = new Stack<>();
    Map<String, Double> mapWithVariables = new HashMap<>();

    public Stack<Double> getStack()
    {
        return calcStack;
    }
    public Map<String, Double> getVariables()
    {
        return mapWithVariables;
    }
}