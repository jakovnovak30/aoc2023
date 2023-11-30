module Main where

import qualified Util

-- test solution
solution :: [String] -> [String]
solution = filter (/= "")

main :: IO ()
main = do
    x <- Util.getExample
    print $ solution x
    x <- Util.getTest
    print $ solution x
