#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;
int real_source;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }
      for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            preferences[i][j] = 0;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                j=j-1;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for(int k=0; k<candidate_count; k++)
    {
       int value=strcmp(name,candidates[k]);
       if(value==0)
       {
           ranks[rank]=k;
           return true;

       }

    }


    // TODO
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
  for(int i=0;i<candidate_count-1;i++)
  {
     int value[i];
    for(int k=0;k<i;k++)
    {
        value[k]=ranks[k];
    }
        for(int j=0;j<candidate_count;j++)
        {
          int real_value=0;
          for(int b=0;b<i;b++)
          {
              if(j==value[b])
              {
                  real_value=1;
              }

          }
          if(j!=ranks[i]&&real_value!=1)
          {
               preferences[ranks[i]][j]+=1;
          }
          real_value=0;
        }

}
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for(int i=0; i<candidate_count; i++)
    {
        int price=0;
        for(int j=0; j<candidate_count; j++)
        {
            if(i!=j&&pair_count<candidate_count * (candidate_count - 1) / 2&&i<j)
            {
                if(preferences[i][j]>preferences[j][i])
                {
                    pairs[price].winner = i;
                    pairs[price].loser = j;
                    pair_count+=1;
                    price+=1;


                }
                else
                {
                    pairs[price].winner = j;
                    pairs[price].loser = i;
                    pair_count+=1;
                    price+=1;
                }
            }


        }

    }
    // TODO
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int t[pair_count];
    int s[pair_count];
    for(int i =0; i<pair_count; i++)
    {
        int k =pairs[i].winner;
        int l =pairs[i].loser;
        t[i] =preferences[k][l]-preferences[l][k];
        s[i] =preferences[k][l]-preferences[l][k];

    }
    //sorting s
     for(int i=0; i<pair_count; i++)
    {
       int position =i;
       int max=0;
       for(int j=i; j<pair_count;j++)
       {
           if(max<s[j])
           {
               max=s[j];
               position=j;
           }
       }
       s[position]=s[i];
       s[i]=max;
    }
    for(int i=0; i<pair_count; i++)
    {

        for(int j=i; j<pair_count; j++)
        {
            int k=pairs[j].winner;
            int l=pairs[j].loser;
            if(t[j]==s[i])
            {
                pairs[j].winner=pairs[i].winner;
                pairs[j].loser=pairs[i].loser;
                pairs[i].winner=k;
                pairs[i].loser=l;
            }
        }
    }
    // TODO
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for(int i=0; i<pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser]= true;
    }
     real_source=0;
    while(real_source==0)
    {

    for(int i=0; i<pair_count; i++)
    {
       int source=i+1;
        for(int j=0; j<pair_count; j++)
        {
          if(i!=j)

          {
              if(locked[i][j]==false)
            {
               source=0;
            }
          }
        }
        if(source!=0)
        {
            real_source=i+1;
        }
        source=0;

    }
    if(real_source==0)
    {
        locked[pairs[pair_count-1].winner][pairs[pair_count-1].loser]=false;
    }
    }
    // TODO
    return;
}

// Print the winner of the election
void print_winner(void)
{
    printf("%s",candidates[real_source-1]);
    // TODO
    return;
}
