# GPU_Tree

<strong>Author:</strong> Aleix Rius <br>
<strong>Version:</strong> 1.0 <br>
<strong>License:</strong> GNU GPLv3

## About
<p> This project is an attempt to process multiple L-System using a GPU. The base idea consist in divide the tree structre into pieces of
wood or "chunks". A group of chunks will form a branch, and a group of branch will form a tree. </p> 

<p> Every Chunks are calculated in BranchSpace Coordinates, meanwhile branches are independant and can be attached in any tree, giving the
oportunity to perfom some randomess at the tree-creation stage.<br> 
Nevertheless this version does not apply that random algorithm, it just follows the L-System syntax similar as tortoise Algorithm, but taking an advantage of the independece of the branches to perfom paral·lel 
calculations.</p>

## Compiling

<p> In order to compile the code is needed Visual Studio v15, and compile for <strong>32 bits (x86)</strong> since some librarys needs to be on 32 bits. 
  In addition, the first compile you will get an error about missing Assimpt library. You can find it under <strong>Llibreries/Assimp</strong> folder. Just Copy the library to the new created Debug Folder on the root project. <p> 
