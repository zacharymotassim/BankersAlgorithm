#include<iostream>
#include<vector>
#include<set>
// Author: Zachary Motassim
// the following file contains procedures stimulating Bankers Algorithm in c++
//procedutre to print a matrix
void PrintMatrix(std::vector<std::vector<int>>&v,std::string catergory)
{
  std::cout<<catergory<<'\n';
  for(size_t i(0);i<v.size();++i)
  {
    for(size_t j(0);j<v[i].size();++j)
    {
      std::cout<<v[i][j]<<' ';
    }
    std::cout<<'\n';
  }
  std::cout<<'\n';
}
//procedure to print array
template<typename Object>
void PrintArray(std::vector<Object>&v)
{
  for(auto i:v)
  {
    std::cout<<i<<' ';
  }
  std::cout<<'\n';
}

// calculates need matrix which represent how many instances of each resource a process needs
std::vector<std::vector<int>>NeedMatrix(std::vector<std::vector<int>>&alloc_,std::vector<std::vector<int>>&max_)
{
  if(alloc_.size()!=max_.size()||alloc_[0].size()!=max_[0].size())
  {throw std::runtime_error("Must be equal size marices.\n");}
  std::vector<std::vector<int>>need;
  for(size_t i(0);i<alloc_.size();++i)
  {
    std::vector<int>m;
    for(size_t j(0);j<alloc_[i].size();++j)
    {
      m.push_back(max_[i][j]-alloc_[i][j]);//the amount it will need overall minus how many it has thus far
    }
    need.push_back(m);
  }
  return need;
}

// finds a process that can be serviced
bool CanBeServed(std::vector<int>&avail_row,std::vector<int>&need_row)
{
  for(size_t i(0);i<avail_row.size();++i)
  {
    if(need_row[i]>avail_row[i])// if it is asking for more instances of a resource that is currently available return false
    {
      return false;
    }
  }
  return true;
}

// executes bankers algorithm and prints the safe sequence
std::vector<std::vector<int>>BankersAlgorithm(std::vector<std::vector<int>>&alloc_,std::vector<std::vector<int>>&need_,std::vector<int>&avail)
{
  size_t avail_pos(0),need_index(0);
  std::vector<std::vector<int>>available_matrix;//matrix to be returned
  available_matrix.push_back(avail);
  std::set<size_t>seen;//holds index of already serviced processes
  std::vector<size_t>safe_sequence;//store the safe sequence i.e the sequence of processes that can be executed without deadlock
    while(need_index<need_.size())
    {// check if can be served and make sure it hasnt already beed serviced
      if(CanBeServed(available_matrix[avail_pos],need_[need_index]) && !seen.count(need_index))
      {
        std::vector<int>row;
        for(size_t j(0);j<need_[need_index].size();++j)
        {//taking reources back after being service. addingback what it had sllocated to resources available
          row.push_back(available_matrix[avail_pos][j]+alloc_[need_index][j]);
        }
        avail_pos++;//move one spot in available array
        seen.insert(need_index);//insert pos of serviced process into set to avoid double servicing it
        available_matrix.push_back(row);//push back updated amount of processes available into available matrix
        safe_sequence.push_back(need_index);//to displaye safe sequence
        need_index=-1;//reset need_index
      }
      need_index++;//move to next spot in need array
    }
    if(available_matrix.size()!=need_.size()+1)
    {std::cout<<"No safe sequence exists\n";}
    else{
    std::cout<<"The safe sequence is: ";
    PrintArray(safe_sequence);
  }
  return available_matrix;
}
// builds matrix from user input
std::vector<std::vector<int>>FillUserInput()
{
  std::vector<std::vector<int>>return_matrix;
  size_t row(0),col(0);
  std::cout<<"Enter row length\n";
  std::cin>>row;
  std::cout<<"Enter column length\n";
  std::cin>>col;
  size_t curr_row(0),curr_col(0),temp(0);
  while(curr_col!=col){
  std::cout<<"Enter row: "<<temp<<'\n';
  temp++;
  std::vector<int>row_;
  while(curr_row!=row)
  {
    int element(0);
    std::cin>>element;
    row_.push_back(element);
    curr_row++;
  }
  curr_row=0;
  return_matrix.push_back(row_);
  curr_col++;
  }
  return return_matrix;
}
// counts the total amount of resources in the system
std::vector<int>TotalResources(std::vector<std::vector<int>>&alloc,std::vector<int>&avail)
{
  std::vector<int>return_vec;
  int sum(0);
  size_t col_index(0),row_index(0);
  while(col_index!=avail.size())
  {
    while(row_index!=alloc.size())
    {
    sum+=alloc[row_index][col_index];
    row_index++;
    }
  sum+=avail[col_index];
  return_vec.push_back(sum);
  col_index++;
  sum=0;
  row_index=0;
}
  return return_vec;
}
// establish initial amount of available resources
std::vector<int>AvailCol()
{
  std::vector<int>avail;
  std::cout<<"Input row size:\n";
  int row_size(0),inc(0);
  std::cin>>row_size;
  std::cout<<"Enter elements:\n";
  while(inc!=row_size)
  {
    int num(0);
    std::cin>>num;
    avail.push_back(num);
    inc++;
  }
  return avail;
}
// wrapper procedure
void ExecuteBankers()
{
  std::cout<<"Build allocation matrix:\n";
  std::vector<std::vector<int>>alloc_=FillUserInput();
  std::cout<<"Build max matrix:\n";
  std::vector<std::vector<int>>max_=FillUserInput();
  std::cout<<"Establish initial available resources:\n";
  std::vector<int>initally_available=AvailCol();
  std::vector<std::vector<int>>need_=NeedMatrix(alloc_,max_);
  std::vector<std::vector<int>>available_matrix=BankersAlgorithm(alloc_,need_,initally_available);
  std::cout<<"Total resources:\n";
  std::vector<int>total_resources=TotalResources(alloc_,initally_available);
  PrintArray(total_resources);
  PrintMatrix(alloc_,"allocation");
  PrintMatrix(max_,"max");
  PrintMatrix(available_matrix,"available");
  PrintMatrix(need_,"need");
}
int main()
{
  ExecuteBankers();
  return 0;
}
