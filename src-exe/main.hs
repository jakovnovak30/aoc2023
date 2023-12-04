module Main where

import qualified Util
import Day4

main :: IO ()
main = do
    x <- Util.getTest
    print $ solution2 x
