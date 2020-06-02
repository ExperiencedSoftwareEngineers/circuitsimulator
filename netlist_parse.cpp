#include "netlist_parse.cpp"

//Splits a single component line into individual vectors
vector<string> next()
{
    string line;
    vector<string> vector_line;

    getline(src, line);

    line += ' ';
    int prevBreak = 0;

    for (int i = 0; i <= line.length(); i++)
    {
        if (line[i] == ' ' || line[i] == '\n')
        {
            string word;

            for (int z = prevBreak; z < i; z++)
            {
                word = word + line[z];
            }

            vector_line.push_back(word);
            prevBreak = i+1;

        }
    }

    return vector_line;
}