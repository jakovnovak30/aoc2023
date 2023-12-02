module Main where

import qualified Util
import Day2

main :: IO ()
main = do
    x <- Util.getExample
    print $ solution x
