module Util (getExample, getTest) where

getInputLines :: FilePath -> IO [String]
getInputLines filename = lines <$> readFile filename

getExample :: IO [String]
getExample = getInputLines "./src-exe/example.txt"

getTest :: IO [String]
getTest = getInputLines "./src-exe/input.txt"
