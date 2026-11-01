#include <iostream>
using namespace std;
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>

enum enQuestionLevel
{
    Easy = 1,
    Med = 2,
    Hard = 3,
    Mix = 4
};

enum enOperationType
{
    Add = 1,
    Sub = 2,
    Mult = 3,
    Div = 4,
    Mixop = 5
};

struct stQuestionInfo
{
    int Num1 = 0;
    int Num2 = 0;
    enQuestionLevel QuestionLevel;
    enOperationType opType;
    int correctAnswer = 0;
    int UserAnswer = 0;
    bool checkAnswer = false;
};

struct stQuizzInfo
{
    stQuestionInfo Question[100];
    int NumberOfQuestions = 0;
    enQuestionLevel QuestionLevel;
    enOperationType opType;
    short RightAnswers = 0;
    short WrongAnswers = 0;
    bool isPass = false;
};

int RandomNum(int From, int To)
{
    int RandNum = rand() % (To - From + 1) + From;
    return RandNum;
}

short ReadHowManyQuestions()
{
    short Questions = 0;

    do
    {
        cout << "How Many Questions you want to ask? (1-100)\n";
        cin >> Questions;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            Questions = 0;
        }

    } while (Questions < 1 || Questions > 100);

    return Questions;
}

enQuestionLevel GetQuestionLevel()
{
    short QuestionLevel;

    do
    {
        cout << "Enter question level [1] Easy, [2] Med, [3] Hard, [4] Mix?\n";
        cin >> QuestionLevel;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            QuestionLevel = 0;
        }

    } while (QuestionLevel < 1 || QuestionLevel > 4);

    return (enQuestionLevel)QuestionLevel;
}

enOperationType GetOperationType()
{
    int OpType;

    do
    {
        cout << "Enter Operation type [1] Add, [2] Sub, [3] Mult, [4] Div, [5] Mix?\n";
        cin >> OpType;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            OpType = 0;
        }

    } while (OpType < 1 || OpType > 5);

    return (enOperationType)OpType;
}

string GetOpTypeSymbol(enOperationType OpType)
{
    switch (OpType)
    {
    case enOperationType::Add:
        return "+";
    case enOperationType::Sub:
        return "-";
    case enOperationType::Mult:
        return "x";
    case enOperationType::Div:
        return "/";
    default:
        return "Mix";
    }
}

int simpleCalculater(int Num1, int Num2, enOperationType opType)
{
    switch (opType)
    {
    case enOperationType::Add:
        return Num1 + Num2;
    case enOperationType::Sub:
        return Num1 - Num2;
    case enOperationType::Div:
        return Num1 / Num2;
    case enOperationType::Mult:
        return Num1 * Num2;
    default:
        return Num1 - Num2;
    }
}

stQuestionInfo GenerateQuestion(enQuestionLevel QuestionLevel, enOperationType opType)
{
    stQuestionInfo Question;

    if (QuestionLevel == enQuestionLevel::Mix)
    {
        QuestionLevel = (enQuestionLevel)RandomNum(1, 3);
    }

    if (opType == enOperationType::Mixop)
    {
        opType = (enOperationType)RandomNum(1, 4);
    }

    Question.opType = opType;

    int From = 1, To = 10;

    switch (QuestionLevel)
    {
    case enQuestionLevel::Easy:
        From = 1;
        To = 10;
        break;

    case enQuestionLevel::Med:
        From = 11;
        To = 50;
        break;

    case enQuestionLevel::Hard:
        From = 51;
        To = 100;
        break;

    case enQuestionLevel::Mix:
        // Mix is normally resolved to Easy, Med, or Hard above.
        From = 1;
        To = 10;
        break;
    }

    Question.Num1 = RandomNum(From, To);
    Question.Num2 = RandomNum(From, To);

    // Generate an exact division question to avoid unintended integer truncation.
    if (opType == enOperationType::Div)
    {
        while (Question.Num1 % Question.Num2 != 0)
        {
            Question.Num1 = RandomNum(From, To);
        }
    }

    Question.QuestionLevel = QuestionLevel;
    Question.correctAnswer = simpleCalculater(Question.Num1, Question.Num2, opType);

    return Question;
}

void GenerateQuizzQuestions(stQuizzInfo& Quizz)
{
    for (int i = 0; i < Quizz.NumberOfQuestions; i++)
    {
        Quizz.Question[i] = GenerateQuestion(Quizz.QuestionLevel, Quizz.opType);
    }
}

void printQuestions(stQuizzInfo Quizz, int QuestionNumber)
{
    cout << "\n";
    cout << "Question [" << QuestionNumber + 1 << "/" << Quizz.NumberOfQuestions << "]\n\n";
    cout << Quizz.Question[QuestionNumber].Num1 << endl;
    cout << Quizz.Question[QuestionNumber].Num2 << " ";
    cout << GetOpTypeSymbol(Quizz.Question[QuestionNumber].opType);
    cout << "\n_____________\n";
}

int playerAnswer()
{
    int answer = 0;
    cin >> answer;
    return answer;
}

void showColorScreen(bool answer)
{
    switch (answer)
    {
    case true:
        system("color 2F");
        break;

    default:
        cout << "\a";
        system("color 4F");
        break;
    }
}

void correctQuestionAnswer(stQuizzInfo& Quizz, int QuestionNumber)
{
    if (Quizz.Question[QuestionNumber].UserAnswer != Quizz.Question[QuestionNumber].correctAnswer)
    {
        Quizz.Question[QuestionNumber].checkAnswer = false;
        cout << "Wrong Answer!\n";
        cout << "The right answer is: " << Quizz.Question[QuestionNumber].correctAnswer << endl;
        Quizz.WrongAnswers++;
    }
    else
    {
        cout << "Right Answer.\n";
        Quizz.Question[QuestionNumber].checkAnswer = true;
        Quizz.RightAnswers++;
    }

    showColorScreen(Quizz.Question[QuestionNumber].checkAnswer);
}

void AskAndCorrectQuestionListAnswers(stQuizzInfo& Quizz)
{
    for (int QuestionNumber = 0; QuestionNumber < Quizz.NumberOfQuestions; QuestionNumber++)
    {
        printQuestions(Quizz, QuestionNumber);
        Quizz.Question[QuestionNumber].UserAnswer = playerAnswer();
        correctQuestionAnswer(Quizz, QuestionNumber);
    }

    Quizz.isPass = (Quizz.WrongAnswers < Quizz.RightAnswers);
}

string GetPassOrFailSympol(stQuizzInfo GameResults)
{
    if (GameResults.isPass)
    {
        return "Pass";
    }
    else
        return "Fail!";
}

string GetQuestionLevelText(enQuestionLevel QuestionLevel)
{
    string arrQuestionLevelText[4] = {"Easy", "Med", "Hard", "Mix"};
    return arrQuestionLevelText[QuestionLevel - 1];
}

void PrintFinalGameResults(stQuizzInfo GameResults)
{
    cout << "\n\nFinal Results is       :" << GetPassOrFailSympol(GameResults) << endl;
    cout << "Number of questions    : " << GameResults.NumberOfQuestions << endl;
    cout << "Question Level         : " << GetQuestionLevelText(GameResults.QuestionLevel) << endl;
    cout << "OpType                 : " << GetOpTypeSymbol(GameResults.opType) << endl;
    cout << "Number of Right Answers: " << GameResults.RightAnswers << endl;
    cout << "Number of Wrong Answers: " << GameResults.WrongAnswers << endl;
    cout << "\n________________________________\n\n";
}

void PlayMatchGame()
{
    stQuizzInfo Quizz;

    Quizz.NumberOfQuestions = ReadHowManyQuestions();
    Quizz.opType = GetOperationType();
    Quizz.QuestionLevel = GetQuestionLevel();
    GenerateQuizzQuestions(Quizz);
    AskAndCorrectQuestionListAnswers(Quizz);
    PrintFinalGameResults(Quizz);
}

void ResetScreen()
{
    system("cls");
    system("color 0F");
}

void startGame()
{
    char AgainPlay = 'y';

    do
    {
        ResetScreen();
        PlayMatchGame();

        cout << "Do you want to play again? Y/N?";
        cin >> AgainPlay;

    } while (AgainPlay == 'y' || AgainPlay == 'Y');
}

int main()
{
    srand((unsigned)time(NULL));
    startGame();
}
